from IPython.display import SVG, display

# Load the saved SVG file and display it
with open('output.svg', 'rb') as f:
    display(SVG(f.read()))