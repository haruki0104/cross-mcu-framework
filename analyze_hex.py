#!/usr/bin/env python3
"""
TI-TXT HEX 檔案分析工具
分析 TI C2000 生成的 HEX 檔案格式
"""

def analyze_ti_hex(filename):
    print(f"=== 分析 {filename} ===")
    
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
        
        print(f"總行數: {len(lines)}")
        print(f"檔案大小: {sum(len(line.encode()) for line in lines)} bytes")
        
        # 分析每一行
        total_data_bytes = 0
        address_ranges = []
        
        for i, line in enumerate(lines[:10]):  # 只顯示前10行
            line = line.strip()
            if line.startswith('%'):
                # TI-TXT 格式分析
                # 格式: %LLAAAAAADD...DD
                # LL = 長度 (hex)
                # AAAAAA = 位址 (hex) 
                # DD...DD = 資料 (hex)
                
                if len(line) >= 9:
                    length_hex = line[1:3]
                    addr_hex = line[3:9]
                    data_hex = line[9:]
                    
                    try:
                        length = int(length_hex, 16)
                        address = int(addr_hex, 16)
                        data_bytes = len(data_hex) // 2
                        
                        print(f"行 {i+1:2d}: 位址=0x{address:06X}, 長度={length:2d}, 資料={data_bytes:2d}bytes")
                        
                        total_data_bytes += data_bytes
                        address_ranges.append((address, address + data_bytes - 1))
                        
                    except ValueError:
                        print(f"行 {i+1:2d}: 格式錯誤")
                else:
                    print(f"行 {i+1:2d}: 長度不足")
            else:
                print(f"行 {i+1:2d}: 非標準格式: {line[:20]}...")
        
        if len(lines) > 10:
            print(f"... (還有 {len(lines) - 10} 行)")
        
        print(f"\n總資料位元組: {total_data_bytes}")
        
        if address_ranges:
            min_addr = min(addr[0] for addr in address_ranges)
            max_addr = max(addr[1] for addr in address_ranges)
            print(f"位址範圍: 0x{min_addr:06X} - 0x{max_addr:06X}")
            print(f"記憶體跨度: {max_addr - min_addr + 1} bytes")
            
            # 檢查是否為 Flash 位址
            if min_addr >= 0x080000:
                print("✅ Flash 記憶體位址 (可燒錄)")
            else:
                print("⚠️  非標準 Flash 位址")
        
    except FileNotFoundError:
        print(f"❌ 檔案不存在: {filename}")
    except Exception as e:
        print(f"❌ 分析錯誤: {e}")

if __name__ == "__main__":
    import sys
    
    hex_files = [
        "build/TI_C2000_F28P55X_Debug/bin/gpio_blink_example.hex",
        "build/TI_C2000_F28P55X_Debug/bin/uart_echo_example.hex"
    ]
    
    for hex_file in hex_files:
        analyze_ti_hex(hex_file)
        print()
