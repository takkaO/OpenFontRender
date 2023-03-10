import re
import glob
import xml.etree.ElementTree as ET 

def main():
	modifyIndexHtml()
	updateKeywordTxt()
	updateLibraryProperty()

def updateLibraryProperty():
	print("\tUpdate 'library.property' ... ", end="")

	tree = ET.parse("../xml/class_open_font_render.xml") 
	root = tree.getroot()

	main_version = re.search(r"\d+", root.find(".//memberdef[name='MAIN_VERSION']").find("initializer").text).group()
	minor_version = re.search(r"\d+", root.find(".//memberdef[name='MINOR_VERSION']").find("initializer").text).group()

	txt = ""
	with open("../library.properties", "r", encoding="utf8") as f:
		txt = f.read()
		txt = re.sub(r"version=.*", "{0}{1}.{2}.0".format(r"version=", main_version, minor_version), txt)
	
	with open("../library.properties", "w+", encoding="utf8") as f:
		f.write(txt)

	print("Done!")

def updateKeywordTxt():
	print("\tUpdate 'keyword.txt' ... ", end="")
	lst = glob.glob("../xml/class_*.xml")

	with open("../keywords.txt", "w", encoding="utf8") as f:
		for target in lst:
			## Parse xml
			tree = ET.parse(target) 
			root = tree.getroot()
			pub_classes = [e.find("compoundname").text for e in root.findall(".//compounddef[@prot='public'][@kind='class']")]
			pub_funcs = [e.find("name").text for e in root.findall(".//memberdef[@prot='public'][@kind='function']")]
			pub_literals = [e.find("name").text for e in root.findall(".//memberdef[@prot='public'][@kind='variable']")]

						
			f.write("##############################################\n")
			f.write("# Syntax Coloring Map OpenFontRender library\n")
			f.write("##############################################\n\n")

			f.write("##############################################\n")
			f.write("# Datatypes (KEYWORD1)\n")
			f.write("##############################################\n")

			for elem in pub_classes:
				f.write("{0}\tKEYWORD1\n".format(elem))
			f.write("\n")

			f.write("##############################################\n")
			f.write("# Methods and Functions (KEYWORD2)\n")
			f.write("##############################################\n")

			for elem in pub_funcs:
				if elem in pub_classes:
					continue
				f.write("{0}\tKEYWORD2\n".format(elem))
			f.write("\n")
			
			f.write("##############################################\n")
			f.write("# Constants (LITERAL1)\n")
			f.write("##############################################\n")
			for elem in pub_literals:
				if elem in pub_classes:
					continue
				f.write("{0}\tLITERAL1\n".format(elem))
			f.write("\n")
	print("Done!")


def modifyIndexHtml():
	print("\tModify 'index.html' ... ", end="")
	target = "../docs/index.html"

	html = ""
	with open(target, "r", encoding="utf8") as f:
		html = f.read()
	#print(html)
	with open(target, "w+", encoding="utf8") as f:
		html = re.sub(r"<a class=\"code.*>(.*)</a>", r"\1", html)
		f.write(html)
	print("Done!")
	


if __name__ == "__main__":
	print("After process start")
	main()
	print("After process finish")