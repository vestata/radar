#!/bin/bash

# 啟動 Radar 程式並將輸出傳送給 Python WebSocket 客戶端
sudo ./build/bin/BGT60LTR11AIP_raw_data_ltr11 | ./client

# 啟動 ADXL355 程式並將輸出傳送給 Python WebSocket 客戶端
# python3 -u adxl355-python/examples/current_values_loop.py | python3 -u websocket_client.py adxl355