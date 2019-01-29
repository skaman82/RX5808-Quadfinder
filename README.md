# RX5808-Quadfinder
Modified RX5808 Pro firmware for my Quadfinder project

<img src="https://raw.githubusercontent.com/skaman82/RX5808-Quadfinder/master/images/start.jpg">

Changes to the original Version: <br>
• UI overhaul<br>
• 1S voltage monitoring, low-voltage alarm, battery icon in the search UI<br>
• RSSI value in search mode<br>
• RSSI buzzer & LED beacon (configurable in search menu)<br><br>

# Different PCB power options<br>
1s lipo > 5V stepup U3V12F5<br>
2-3s lipo > 5V step-up/down S9V11F5 plus voltage divider R7(18k)+R8(4.7k) > !!!not yet usable, but soon!!!<br><br>

<img src="https://raw.githubusercontent.com/skaman82/RX5808-Quadfinder/master/images/pcbs.jpg">

# Materials<br>
Arduino Mini<br>
1.3" OLED I2C Display<br>
3x 6x6x9mm tact buttons<br>
RX5808 SPI Module<br>
5V stepup U3V12F5<br>
3.5mm AV socket<br>
JST-SH 6pin socket<br>
1s lipo<br>
2x 1206 LEDs (D1, D2)<br>
2x 330 0603 Resistor(R5, R6)<br>
3x 1k 0603 Resistor (R1, R2, R3)<br>
100k 0603 Resistor (R4)<br>
100nF 0603 cap (C1)<br>
10uF 1206 tantalum cap (c2) - optional, bridge otherwise.<br>
Mini slide switch: <a href="https://de.aliexpress.com/item/20-st-cke-MSS22D18-Mini-DIP-Schiebeschalter-1P2T-3Pin-f-r-DIY-Elektronische-Zubeh-r-Mikroschalter/32859694454.html?spm=a2g0s.9042311.0.0.27424c4ddocSic">Link</a><br>
500mAh Lipo charger: https://www.banggood.com/Lantian-1S-3_7V-4_2V-0_4A-Android-Micro-USB-Lipo-Battery-Charger-Board-Module-for-FPV-RC-Drone-p-1347517.html?p=03160313547712015036<br>
4x M3 brass inserts and 4x M3 screws<br>
