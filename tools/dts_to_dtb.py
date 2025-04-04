import subprocess
import os
from pathlib import Path

def find_dts_files(directory='.'):
    """
    查找指定目录下的所有.dts文件
    """
    dts_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.dts'):
                dts_files.append(os.path.join(root, file))
    return dts_files

def compile_dts_to_dtb(dts_file):
    """
    使用 dtc 工具将DTS文件编译为DTB文件。
    """
    dtb_file = dts_file.replace('.dts', '.dtb')
    command = ['dtc', '-I', 'dts', '-O', 'dtb', '-o', dtb_file, dts_file]
    
    try:
        subprocess.run(command, check=True)
        print(f"成功编译: {dts_file} -> {dtb_file}")
        return dtb_file
    except subprocess.CalledProcessError as e:
        print(f"编译失败: {e}")
        return None
    except FileNotFoundError:
        print("错误: 未找到dtc工具，请确保设备树编译器已安装")
        return None

def convert_dtb_to_c_array(dtb_file):
    """
    将DTB文件转换为C语言数组格式。
    """
    try:
        with open(dtb_file, 'rb') as f:
            data = f.read()

        # 从DTB文件名提取基本名称（不含路径和扩展名）
        base_name = os.path.splitext(os.path.basename(dtb_file))[0]           
        # 转换为C数组格式
        c_array = ', '.join(f'0x{byte:02x}' for byte in data)
        c_array = f"const unsigned char {base_name}_dtb[] = {{\n    {c_array}\n}};"
        c_array = f'#include "machine/machine.h"\n{c_array}\nconst unsigned int {base_name}_dtb_size = sizeof({base_name}_dtb);'
        return c_array
    except Exception as e:
        print(f"读取DTB文件失败: {e}")
        return None

def save_c_array(c_array, original_dts_file):
    """
    将C数组保存到与DTS文件同目录的同名.c文件中
    """
    c_file = original_dts_file.replace('.dts', '.c')
    try:
        with open(c_file, 'w') as f:
            f.write(c_array)
        print(f"成功生成C文件: {c_file}")
        return c_file
    except Exception as e:
        print(f"保存C文件失败: {e}")
        return None

def process_dts_file(dts_file):
    """
    处理单个DTS文件：编译为DTB，转换为C数组，并保存
    """
    print(f"\n处理文件: {dts_file}")
    dtb_file = compile_dts_to_dtb(dts_file)
    if dtb_file:
        c_array = convert_dtb_to_c_array(dtb_file)
        if c_array:
            save_c_array(c_array, dts_file)
        # 清理临时生成的DTB文件
        try:
            os.remove(dtb_file)
            print(f"已删除临时文件: {dtb_file}")
        except Exception as e:
            print(f"删除临时文件失败: {e}")

def main():
    # 查找当前目录下的所有DTS文件
    dts_files = find_dts_files()
    
    if not dts_files:
        print("未找到任何.dts文件")
        return
    
    print(f"找到 {len(dts_files)} 个.dts文件:")
    for file in dts_files:
        print(f"  - {file}")
    
    # 处理每个DTS文件
    for dts_file in dts_files:
        process_dts_file(dts_file)

if __name__ == "__main__":
    main()
