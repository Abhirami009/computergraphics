from PIL import Image

# Open the JPG image
img = Image.open("texture2.jpeg")

# Convert and save as BMP
img.save("texture2.bmp")
