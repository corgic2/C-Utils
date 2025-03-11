# 添加VS过滤器
function(source_group_by_dir source_files)
    if(MSVC)
        set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(file ${${source_files}})
            string(REGEX REPLACE ${cur_dir}/\(.*\) \\1 file_path ${file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${file_path})
            string(COMPARE EQUAL ${file_path} ${sgbd_group_name} nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            get_filename_component(file_extension ${file} EXT)
            if(file_extension STREQUAL ".ui")                   # ui文件，放Form Files
                source_group("Form Files" FILES ${file})
            elseif(file_extension STREQUAL ".qrc")
                source_group("Resource Files" FILES ${file})    # qrc文件，放Resource Files
				elseif(file_extension STREQUAL ".ts")
                source_group("Language" FILES ${file})    # qrc文件，放Language
            elseif(NOT nogroup)
                source_group(${sgbd_group_name} FILES ${file})  # 使用所在子目录名作为过滤器名
            endif()
        endforeach(file)
    endif(MSVC)
endfunction(source_group_by_dir)

# 生成后复制事件
function(after_build_copy target_name copy_src copy_dest)
    # unix path转为windows path
    string(REGEX REPLACE "/" "\\\\" copy_dest_win ${copy_dest})
    string(REGEX REPLACE "/" "\\\\" copy_src_win ${copy_src})
    message("-----------COPY------------")
    message("---------src: " ${copy_src_win})
    message("---------dst: " ${copy_dest_win})
    add_custom_command(TARGET ${target_name} POST_BUILD COMMAND Copy /Y "${copy_src_win}" "${copy_dest_win}")
endfunction(after_build_copy target_name copy_src copy_dest)

# 自动解析props,把所有存在的dll拷贝过来
function(parse_props_copy_dll PROPS_LIST CMAKE_CONFIGURATION_TYPES PLATFORM_CPU SRC_PATH OUTPUT_EXE_DIR)
	set(PROPS_CONDITION "'`$(Configuration)|`$(Platform)'=='${CMAKE_CONFIGURATION_TYPES}|${PLATFORM_CPU}'")
	foreach(oneProps ${${PROPS_LIST}})
		# 使用 get_filename_component 获取文件名
		get_filename_component(FILENAME ${oneProps} NAME)
		if(EXISTS "${oneProps}")
			# 使用 PowerShell 脚本来解析XML文件并提取节点中有多少lib引用
			execute_process(
				COMMAND powershell -Command "$xml = [xml](Get-Content '${oneProps}'); $xml.Project.ItemDefinitionGroup | Where-Object { $_.Condition -eq \"${PROPS_CONDITION}\"} | ForEach-Object { $_.Link.AdditionalDependencies }"
				OUTPUT_VARIABLE ADDITIONAL_DEPENDENCIES
				OUTPUT_STRIP_TRAILING_WHITESPACE
			)
			if(ADDITIONAL_DEPENDENCIES)
				# 使用正则表达式匹配库名
				string(REGEX MATCHALL "([^\;]+)\\.lib" PROPS_LIBRARIES ${ADDITIONAL_DEPENDENCIES})
				# 检查是否匹配到了结果
				if(PROPS_LIBRARIES)
					string(REPLACE ".lib" ";" ARR_LIB_NAMES ${PROPS_LIBRARIES})
					# 拷贝lib库名对应的dll到本项目目录下
					foreach(ONE_LIBNAME ${ARR_LIB_NAMES})
						set(COPY_PROPS_DLL "${SRC_PATH}/${PLATFORM_CPU}/${CMAKE_CONFIGURATION_TYPES}/${ONE_LIBNAME}.dll")
						if(EXISTS "${COPY_PROPS_DLL}")
							after_build_copy(${TARGET_NAME} ${COPY_PROPS_DLL} ${OUTPUT_EXE_DIR})
						endif(EXISTS "${COPY_PROPS_DLL}")
					endforeach(ONE_LIBNAME ${ARR_LIB_NAMES})
				endif(PROPS_LIBRARIES)	
			endif(ADDITIONAL_DEPENDENCIES)
		endif(EXISTS "${oneProps}")	
	endforeach(oneProps  ${${PROPS_LIST}})
endfunction(parse_props_copy_dll)