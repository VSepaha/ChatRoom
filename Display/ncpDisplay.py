#!/usr/bin/env python
# Display a runtext with double-buffering.
from samplebase import SampleBase
from rgbmatrix import graphics
import time

fileName = "FinalProject/logfile.txt"

class RunText(SampleBase):
    def __init__(self, *args, **kwargs):
        super(RunText, self).__init__(*args, **kwargs)

    def Run(self):
        offscreenCanvas = self.matrix.CreateFrameCanvas()
        font = graphics.Font()
        font.LoadFont("../../fonts/7x13.bdf")
        textColor = graphics.Color(255, 0, 155)
        pos = offscreenCanvas.width
        fd = open(fileName, 'r')
        #print(self.args)
        
        logLine = fd.readline() #attempt to read line, if end of file, "" is given
        logLine = logLine[:-1]

        while True:

            offscreenCanvas.Clear()
            len = graphics.DrawText(offscreenCanvas, font, pos, 15, textColor, logLine)
            pos -= 1
            if (pos + len < 0):
                pos = offscreenCanvas.width
                logLine = fd.readline() #read another line
                logLine = logLine[:-1]

            time.sleep(0.05)
            offscreenCanvas = self.matrix.SwapOnVSync(offscreenCanvas)
        fd.close()


# Main function
if __name__ == "__main__":
    parser = RunText()
    if (not parser.process()):
        parser.print_help()
