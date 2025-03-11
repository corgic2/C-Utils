import sys
import time
import pyautogui
import keyboard
import pyperclip

def simulate_key_press(key, modifiers=None):
    if modifiers:
        for modifier in modifiers:
            keyboard.press(modifier)
        time.sleep(0.3)  # 短暂延迟以确保按键顺序
    keyboard.press_and_release(key)
    if modifiers:
        time.sleep(0.3)  # 短暂延迟以确保按键顺序
        for modifier in modifiers:
            keyboard.release(modifier)

def simulate_type_file_name(file_name):
    for char in file_name:
        pyautogui.press(char)

def main(file_names,filePath):
    time.sleep(2)
    # 模拟按下 Ctrl + L 切换聊天模式
    simulate_key_press('i', ['ctrl'])
    simulate_key_press('l', ['ctrl'])
    for file_name in file_names:
        # 模拟按下 Ctrl + Alt + P
        simulate_key_press('p', ['ctrl', 'alt'])

        # 输入文件名
        simulate_type_file_name(file_name)

        # 输入回车
        pyautogui.press('enter')

        # 模拟按下 Ctrl + L 恢复初始状态
        simulate_key_press('l', ['ctrl'])

    # 将 Autoui.txt 的内容复制到 Cursor 的聊天框中
    with open(filePath, 'r', encoding='utf-8') as file:
        content = file.read()
        
    pyperclip.copy(content)
    simulate_key_press('i', ['ctrl'])
    simulate_key_press('l', ['ctrl'])
    # 模拟粘贴内容
    pyautogui.hotkey('ctrl', 'v')

    # 输入回车
    pyautogui.press('enter')

if __name__ == "__main__":
    if len(sys.argv) >= 3:
        file_names = sys.argv[1].split('\n')
        filePath = sys.argv[2]
        main(file_names,filePath)
    else:
        print("Usage: python cursor_operations.py <file_names> <html_code> <css_code>")