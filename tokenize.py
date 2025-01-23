def tokenize(string):
	in_string = False
	current_token = ""

	for char in string:
		if in_string:
			if char == "\"":
				in_string = False
				print(repr(current_token))
				current_token = ""
				continue
			current_token += char
			continue
		
		if char == "\"":
			in_string = True
			continue
		elif char == " ":
			print(repr(current_token))
			current_token = " "
			continue
		elif char == "<":
			print(repr(current_token))
			current_token = "<"
			continue
		else:
			current_token += char
	if current_token:
		print(repr(current_token))

tokenize('cat -e " < a"Makefile')