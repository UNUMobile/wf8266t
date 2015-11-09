# WF8266T-Touch 開發板介紹

![](../imgs/DSC02177.JPG)

## 預設電路
因為 TFT 和 Touch 驅動需要 SPI 的腳位，因此尚有以下 ＊ GPIO12 這6個腳位可自訂控制使用。

* GPIO0 : UPDATE(紅色按鍵), T_CLK
* GPIO1 : Tx, T_DOUT
* GPIO2 : T_CS
* GPIO3 : T_IRQ
* GPIO4 : SD Card CS
* GPIO5 : D/C
* *GPIO12: MISO(None)
* GPIO13: MOSI
* GPIO14: SCK
* GPIO15: CS
* GPIO16: LED