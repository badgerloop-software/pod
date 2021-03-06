import os
import xml.etree.ElementTree as XMLElementTree
import generators
import util

directoryPath = os.path.dirname(os.path.abspath(__file__))
data = XMLElementTree.parse(directoryPath + "/data.xml").getroot()

# PlaceholderTupleArrayExample: "[("!!AUTOGENERATED CODE!!", generatorA)]"
def autocode(templateFilePath, outputFilePath, placeholderGeneratorTupleArray):
    # Load and prepare the template with a message to let users know the file is autogenerated
    code =  "/* \n" \
            " * Please note that this is an auto-generated file which is automatically generated whenever a target is built.\n" \
            " */\n\n"

    with open(directoryPath + "/" + templateFilePath, mode="rt") as templateFile:
        for line in templateFile:
            code += line
    if code is "":
        raise Exception("Template file is empty.")
    
    # Locate placeholders and run autocoders
    for t in placeholderGeneratorTupleArray:
        placeholder = "/**" + t[0] + "**/"
        # Find indentation
        indentation = ""
        for line in code.split("\n"):
            if placeholder in line:
                indentation = line[0 : (len(line) - len(line.lstrip()))]
        generator = t[1]
        
        if placeholder not in code:
            raise Exception("Could not find placeholder '" + placeholder + "' in template file " + directoryPath + "/" + templateFilePath + "'")
        generatedCode = "\n/* Autogenerated Code Begins */\n" + (generator(data).strip() or "") + "\n/* Autogenerated Code Ends */\n"
        
        # Apply indentation to generated code
        generatedCode = generatedCode.replace("\n", ("\n" + indentation))

        code = code.replace(placeholder, generatedCode)

    # Create the output directory, if it does not exist
    # This code exists because some auto-coded files need to be put in previously empty folders, and git doesn't track empty folders
    if not os.path.exists(os.path.dirname(directoryPath + "/" + outputFilePath)):
        os.makedirs(os.path.dirname(directoryPath + "/" + outputFilePath))

    # Write code to output
    with open(directoryPath + "/" + outputFilePath, mode="w+") as out:
        out.write(code)

    print("Successfully auto-coded " + directoryPath + "/" + outputFilePath + ".")


###
##  AUTO-CODER CALLS GO HERE
###

# data.h file
autocode("templates/data.template.h", "../embedded/data/include/data.h", [("!!AUTO-GENERATE HERE!!", generators.generateDataHeader)])

# data.c file
autocode("templates/data.template.c", "../embedded/data/src/data.c", [("!!AUTO-GENERATE HERE!!", generators.generateDataC)])

# init.c file
autocode("templates/init.template.c", "../embedded/app/src/init.c", [("!!AUTO-GENERATE HERE!!", generators.generateInitC)])

# TelemetryLoop.cpp
autocode("templates/TelemetryLoop.template.cpp", "../middleware/src/TelemetryLoop.cpp", [("!!AUTO-GENERATE HERE!!", generators.generateBufferContents)])