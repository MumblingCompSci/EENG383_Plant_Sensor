## Display/calculation code for plant monitor.
# Get sensor data from PIC, then calculate their values on a 0-100 scale.
# Finally, display the data (text for the temperature, bars for everything else)
# printed in black if the value is within bounds and red if not
from inky import InkyPHAT
from PIL import Image, ImageFont, ImageDraw
import serial
from time import sleep

# Constants
DISPLAY_DIMS = (212, 104)
LINE_SPACING = 31
WARNING_LOW = {'T': 0,
               'H': 10,
               'L': 10,
               'M': 30}
WARNING_HIGH = {'T': 26,
                'H': 50,
                'L': 90,
                'M': 75}
PIC = serial.Serial('/dev/ttyAMA0', 115200)

# Set up display stuff
display = InkyPHAT('red')
display.set_border(InkyPHAT.BLACK)

# Create image and font stuff
fontRegular = ImageFont.truetype("./fonts/RobotoMono-Regular.ttf", 25)
fontBold = ImageFont.truetype("./fonts/RobotoMono-Bold.ttf", 25)
fontTemp = ImageFont.truetype("./fonts/Roboto-Bold.ttf", 30)

# Get the color (black for normal, red for "warning") of the display text
def getColor(param, params, warning_low, warning_high):
    return InkyPHAT.BLACK if params[param] < warning_high[param] and params[param] > warning_low[param] else InkyPHAT.RED

def addCoords(pos1, pos2):
    return tuple(map(sum, zip(pos1, pos2)))

while(1):
    # Test info. I'll assume a 0-100 scale for now
    params = {'T': 74, #temperature
              'H': 50, #humidity
              'L': 69, #light
              'M': 40} #moisture
    PIC.write(0x1)
    params['H'] = round(int(PIC.read(2).hex(), 16)/655.35)
    print("H: {}".format(params['H']))
    params['L'] = round(int(PIC.read(2).hex(), 16) / 655.35)
    print("L: {}".format(params['L']))
    params['M'] = round(int(PIC.read(1).hex(), 16) / 2.55)
    print("M: {}".format(params['M']))
    params['T'] = round(((175.72*int(PIC.read(2).hex(), 16))/65536) - 46.85, 1)
    print("T: {}".format(params['T']))
    PIC.reset_input_buffer()


    img = Image.new('P', DISPLAY_DIMS, InkyPHAT.WHITE)
    txtLayer = ImageDraw.Draw(img)

    # Draw progress bars for all params but temperature
    for num,param in enumerate(params, start=-1):
        # Skip drawing a bar for temperature: We use text for this
        if param == 'T':
            continue

        startpos = (10, 5+LINE_SPACING*num)
        fillColor = getColor(param, params, WARNING_LOW, WARNING_HIGH)

        # Draw progress bar label
        txtLayer.text(startpos, param, font=fontBold, fill=fillColor)

        # Draw the progress bar bounding rectangles
        rectPos = addCoords(startpos, (20, 8))
        # To achieve an 100px wide rect, we add 102 to the end coords
        # 2px compensation for each 1px wide border, 1px for rectangle() behavior
        # see https://pillow.readthedocs.io/en/stable/reference/ImageDraw.html
        txtLayer.rectangle([rectPos, addCoords(rectPos, (103, 18))], InkyPHAT.WHITE, InkyPHAT.BLACK, 1)

        # Draw the actual progress rectangles
        rectPos = addCoords(rectPos, (1,1)) # make sure we draw _inside_ the bounding rect
        txtLayer.rectangle([rectPos, addCoords(rectPos, (params[param]+1, 16))], fill=fillColor)

    fillColor = getColor('T', params, WARNING_LOW, WARNING_HIGH)
    txtLayer.text((145, 23), str(params['T']) + '\n' + u'\N{DEGREE SIGN}' + 'C', font=fontTemp, fill=fillColor, align='center')

    # Set the image to display, and display it!
    display.set_image(img)
    display.show()
    sleep(15)
