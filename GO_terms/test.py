import re

# Open the text file in read mode
with open('terms.txt', 'r') as file:
    # Read the entire file content
    content = file.read()
    # Search for lines that contain "id:" but not "alt_id:"
    matches = re.findall(r'^.*id:(?!.*alt_id:).*$', content, flags=re.MULTILINE)
    matches2 = re.findall(r'^.*name:(?!.*alt_id:).*$', content, flags=re.MULTILINE)
    matches3 = re.findall(r'^.*is_a:(?!.*alt_id:).*$', content, flags=re.MULTILINE)

    print(len(matches))
    print(len(matches2))
    print(len(matches3))

    
    # Save the matching lines to a file
    with open('output.txt', 'w') as output_file:
        for line1, line2, line3 in zip(matches, matches2, matches3):
            if any(s in line3 for s in ['molecular_function', 'biological_process', 'cellular_component']):
                output_file.write(line1 + ' ' + line2 + ' ' + line3 + '\n')
