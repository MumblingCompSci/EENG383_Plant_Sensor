from inky import InkyPHAT
from PIL import Image, ImageFont, ImageDraw

inkyphat = InkyPHAT('red')
# bg = Image.open("./bg/default.png")
bg = Image.new('P', (212, 104), inkyphat.RED)
bgLayer = ImageDraw.Draw(bg)

inkyphat.set_border(inkyphat.RED)
font = ImageFont.truetype("./fonts/Roboto-Regular.ttf", 15)
# txt = Image.new('P', bg.size, (255,255,255,0))
txtLayer = ImageDraw.Draw(bg)
txtLayer.text((10,10), "howdy", font=font, fill=inkyphat.BLACK)

# out = Image.alpha_composite(bgLayer, txtLayer)

# inkyphat.set_image(txtLayer)
inkyphat.set_image(bg)
inkyphat.show()
