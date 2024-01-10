# IOT Final Project

## 居家智能監測系統

### 112-1 智慧物聯網技術與應用 第9組 期末專案

本專案來自 智慧物聯網技術與應用 National Dong Hwa University CSIE。

## Platform

#### Framework

![image](https://github.com/bryan2641/IOT_Fianl_Project/blob/main/framework.png)

1.  ESP-8266：一款非常流行的低成本Wi-Fi模組
2.	溫溼度感測器：用於監測環境的溫度和濕度，並可以結合其他感測器進行火災的預防
3.	光照感測器：用來檢測環境光線的強度，這可以用於自動調節室內照明
4.	一氧化碳感測器：該感測器可用於及時檢測和警報，防止一氧化碳中毒
5.	可燃氣體感測器：用於檢測天然氣、丙烷等可燃氣體的泄漏，這對於預防火災和爆炸事故至關重要
6.	懸浮微粒感測器：用來監測空氣中的微粒物質，如PM2.5和PM10，這些微粒可能對人體健康構成威脅
7.	蜂鳴器：作為一種聲音報警裝置，當系統檢測到任何異常時，蜂鳴器可以立即發出警報
8.	伺服馬達：這是一種可精確控制的馬達，常用於開啟或關閉閥門、調節裝置位置等
9.	麵包版
10.	樹莓派：作為即時視訊的網站架設平台
11.	攝影機

## Content

`ESP-8266_Code` 裡面是所有ESP-8266的程式碼，包含單獨感測器以及Mesh Network(Root及三個節點)。

`flows.json` 是Node-Red的主結構。

`IOT_Backend` 及 `IOT_RPI` 則是遭棄用的網頁後端框架與樹莓派的程式碼。

## Quick-Start
### 1. Installation

Execute the following command to get the source code.

```shell
git clone https://github.com/bryan2641/IOT_Fianl_Project.git
```

### 2. Execution

* **ESP-8266**：在 Arduino IDE 中開啟 `.ino` 

* **Node-Red**：將 `flows.json` 匯入，選擇流程4，並在節點管理中安裝 `node-red-contrib-line-notify-api` 、 `node-red-dashboard` 、 `node-red-node-serialport` ，並根據 ESP-8266 連接之 Serial Port 選擇對應即可

* **即時串流網頁框架**：
```shell
## Server：
cd /Your_Path/IOT_Backend/

python3 -m venv venv
pip install -r requirements.txt

python.py manage.py runserver


## RPI：
cd /Your_Path/IOT_RPI/

pip install socketIO_client opencv-python

python3 final_demo.py
```

## Contributor
@bryan2641
