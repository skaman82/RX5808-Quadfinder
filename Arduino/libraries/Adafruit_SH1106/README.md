# Adafruit_SH1106


This is fork of SSD1306 from Adafruit for the SH1106.
Some oleds drivers sold as ssd1306 are in fact SH1106 and do not support vertical/horizontal adressing and works only in page adressing (which u8glib uses and therefore u8gib works fine with those oled drivers).

Some other stuff are not supported : eg : scrolling