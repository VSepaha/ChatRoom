#!/usr/bin/env python
# Display a runtext with double-buffering.
from samplebase import SampleBase
from rgbmatrix import graphics
import time

fileName = "logfile.txt" #name of file written by server

messages = 3 #number of messages that are displayed at once

#function for comparing the length of 3 strings
def threeLenCmp(len1, len2, len3):
    if(len1 > len2):
        if(len1 > len3):
            return 1;
        else:
            return 3
    elif(len2 > len3):
        return 2
    else:
        return 3

class RunText(SampleBase):
    def __init__(self, *args, **kwargs):
        super(RunText, self).__init__(*args, **kwargs)

    def Run(self):
        #initialize message position
        offscreenCanvas = self.matrix.CreateFrameCanvas()
        #initialize and set font size
        font = graphics.Font()
        font.LoadFont("../../fonts/7x13.bdf")
        #set prefered text colors
        textColor1 = graphics.Color(255, 155, 0)
        textColor2 = graphics.Color(255, 0, 155)
        textColor3 = graphics.Color(0, 155, 255)
        pos = offscreenCanvas.width
        fd = open(fileName, 'r') #open the log file
        #print(self.args)
        
        logLine1 = fd.readline() #attempt to read line, if end of file, "" is given
        logLine1 = logLine1[:-1]
        logLine1 = logLine1[:-1]

        logLine2 = fd.readline() #attempt to read line, if end of file, "" is given
        logLine2 = logLine2[:-1]
        logLine2 = logLine2[:-1]

        logLine3 = fd.readline() #attempt to read line, if end of file, "" is given
        logLine3 = logLine3[:-1]
        logLine3 = logLine3[:-1]

        #this while loop will continuously check for new messages and will display the
        #new messages as they are written to the log file by the server connection
        while True:

            offscreenCanvas.Clear()
            len1 = graphics.DrawText(offscreenCanvas, font, pos, 10, textColor1, logLine1)
            len2 = graphics.DrawText(offscreenCanvas, font, pos, 20, textColor2, logLine2)
            len3 = graphics.DrawText(offscreenCanvas, font, pos, 30, textColor3, logLine3)

            longLen = threeLenCmp(len1, len2, len3);
            pos -= 1
            #this if/else statement is used to manage when we stop attempting to display to the matrix
            #it tests for the message with the longest length, then prints up to that many characters
            #this is so that messages do not get truncated during display
            if (longLen == 1):
                if (pos + len1 < 0):
                    pos = offscreenCanvas.width
                    logLine1 = fd.readline() #read another line
                    logLine1 = logLine1[:-1]
                    logLine1 = logLine1[:-1]

                    logLine2 = fd.readline()
                    logLine2 = logLine2[:-1]
                    logLine2 = logLine2[:-1]

                    logLine3 = fd.readline()
                    logLine3 = logLine3[:-1]
                    logLine3 = logLine3[:-1]
            elif (longLen == 2):
                if (pos + len2 < 0):
                    pos = offscreenCanvas.width
                    logLine1 = fd.readline() #read another line
                    logLine1 = logLine1[:-1]
                    logLine1 = logLine1[:-1]

                    logLine2 = fd.readline()
                    logLine2 = logLine2[:-1]
                    logLine2 = logLine2[:-1]

                    logLine3 = fd.readline()
                    logLine3 = logLine3[:-1]
                    logLine3 = logLine3[:-1]
            else:
                if (pos + len3 < 0):
                    pos = offscreenCanvas.width
                    logLine1 = fd.readline() #read another line
                    logLine1 = logLine1[:-1]
                    logLine1 = logLine1[:-1]

                    logLine2 = fd.readline()
                    logLine2 = logLine2[:-1]
                    logLine2 = logLine2[:-1]

                    logLine3 = fd.readline()
                    logLine3 = logLine3[:-1]
                    logLine3 = logLine3[:-1]

            time.sleep(0.05)
            #reset message position bach to right side of display
            offscreenCanvas = self.matrix.SwapOnVSync(offscreenCanvas)
        fd.close()


# Main function
if __name__ == "__main__":
    parser = RunText()
    if (not parser.process()):
        parser.print_help()
