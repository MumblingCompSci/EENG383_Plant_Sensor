from inky import InkyPHAT
from PIL import Image, ImageFont, ImageDraw

# Constants
DISPLAY_DIMS = (212, 104)
LINE_SPACING = 31
WARNING_LEVEL = {'T': 50,
           'H': 80,
           'L': 90,
           'M': 75}

# Get the color (black for normal, red for "warning") of the display text
def getColor(param, params, warning):
    return InkyPHAT.BLACK if params[param] < warning[param] else InkyPHAT.RED

def addCoords(pos1, pos2):
    return tuple(map(sum, zip(pos1, pos2)))

# Test info. I'll assume a 0-100 scale for now
params = {'T': 74, #temperature
          'H': 50, #humidity
          'L': 69, #light
          'M': 40} #moisture

# Set up display stuff
display = InkyPHAT('red')
display.set_border(InkyPHAT.BLACK)

# Create image and font stuff
fontRegular = ImageFont.truetype("./fonts/RobotoMono-Regular.ttf", 25)
fontBold = ImageFont.truetype("./fonts/RobotoMono-Bold.ttf", 25)
fontTemp = ImageFont.truetype("./fonts/Roboto-Bold.ttf", 36)
img = Image.new('P', DISPLAY_DIMS, InkyPHAT.WHITE)
# img = Image.open("./bg/inkbg.png")

# Draw example text on image
txtLayer = ImageDraw.Draw(img)

# Start and end of "progress bar"
start = ['[']
stop = [']']

# Draw progress bars for all params but temperature
for num,param in enumerate(params, start=-1):
    # Skip drawing a bar for temperature: We use text for this
    if param == 'T':
        continue

    startpos = (10, 5+LINE_SPACING*num)
    fillColor = getColor(param, params, WARNING_LEVEL)

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

fillColor = getColor('T', params, WARNING_LEVEL)
txtLayer.text((150, 20), str(params['T']) + '\n' + u'\N{DEGREE SIGN}' + 'C', font=fontTemp, fill=fillColor, align='center', spacing=-5)

# Set the image to display, and display it!
display.set_image(img)
display.show()
