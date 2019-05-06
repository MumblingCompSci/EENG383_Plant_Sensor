from inky import InkyPHAT
from PIL import Image, ImageFont, ImageDraw

# Constants
DISPLAY_DIMS = (212, 104)
LINE_SPACING = 20
WARNING_LEVEL = {'T': 50,
           'H': 80,
           'L': 90,
           'M': 75}

# Get the color (black for normal, red for "warning") of the display text
def getColor(param, params, warning):
    return InkyPHAT.BLACK if params[param] < warning[param] else InkyPHAT.RED

# Test info. I'll assume a 0-100 scale for now
params = {'T': 74, #temperature
          'H': 50, #humidity
          'L': 69, #light
          'M': 40} #moisture

# Set up display stuff
display = InkyPHAT('red')
display.set_border(InkyPHAT.BLACK)

# Create image and font stuff
fontRegular = ImageFont.truetype("./fonts/RobotoMono-Regular.ttf", 15)
fontBold = ImageFont.truetype("./fonts/RobotoMono-Bold.ttf", 15)
fontTemp = ImageFont.truetype("./fonts/RobotoMono-Bold.ttf", 25)
img = Image.new('P', DISPLAY_DIMS, InkyPHAT.WHITE)

# Draw example text on image
txtLayer = ImageDraw.Draw(img)
start = ['[']
stop = [']']

# Draw progress bars for all params but temperature
for num,param in enumerate(params, start=-1):
    if param == 'T':
        continue
    label = [param]
    bar = ['|' for i in range(int(params[param]/10))]
    dots = ['.' for i in range(int(params[param]/10), 10)]
    progress = label + start + bar + dots + stop
    txtLayer.text((10,10+LINE_SPACING*num), ''.join(progress), font=fontBold, fill=getColor(param, params, WARNING_LEVEL))

txtLayer.text((150,35), str(params['T']) + u'\N{DEGREE SIGN}' + 'C', font=fontTemp, fill=getColor('T', params, WARNING_LEVEL))

# Set the image to display, and display it!
display.set_image(img)
display.show()
