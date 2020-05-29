import sys



def main():

	#FIND THE INDEX OF THE FILES
	IN_INDEX = 0
	EXC_INDEX = 0
	excluded_words = []	
        
        #FIND INDEX FOR FILES
	if len(sys.argv) == 1:
		print("No file to open")
		exit()
	elif len(sys.argv) == 2:
		IN_INDEX = 1
		EXC_INDEX = None
	elif sys.argv[1] == "-e":
		IN_INDEX = 3
		EXC_INDEX = 2
		excluded_words = store_excluded(EXC_INDEX)
	else:
		IN_INDEX = 1
		EXC_INDEX = 3
		excluded_words = store_excluded(EXC_INDEX)
	
	#CALL FUNCTIONS TO FIND KEYWORD-OUT-OF-CONTEXT
	File, File_lower, words = store_input(IN_INDEX)
	if len(words) == 0:
		exit()
	words = remove_duplicates(words)
	if EXC_INDEX != None:
		words = remove_excluded(words, excluded_words)
	print_output(words, File_lower, File)
	
	

#GRAB WORDS FROM INPUT FILE, CONVERT TO LOWERCASE AND STORE THEM IN ARRAYS
def store_input(IN):
	File = []
	File_lower = []
	words = []
	fp = open(sys.argv[IN], "r")
	for line in fp:
		line = line.rstrip()
		File.append(line)
		File_lower.append(line.lower())
		words.extend(line.split())

	for word in words:
		words[words.index(word)] = word.lower()			
	
	fp.close()
	return File, File_lower, words


#STORE EXCLUDED WORDS IN ARRAY
def store_excluded(EX):
	excluded_words = []
	fp2 = open(sys.argv[EX], "r")
	line = fp2.readline().rstrip()
	while line:
		excluded_words.extend(line.split())
		line = fp2.readline().rstrip()
	fp2.close()
	
	return excluded_words
	

def remove_duplicates(words):	
	words = list(set(words))
	return words
	
def remove_excluded(words, exc_words):
	return [word for word in words if word not in exc_words]
	

def print_output(words, File_lower, File):
	#FIND THE LONGEST WORD
	words = sorted(words, key=len)
	long_word_len = len(words[-1])
	#SORT WORDS IN ALPHABETICAL ORDER
	words.sort()	
		
	for word in words:
		for i in range(0, len(File)):
			count = 0
			temp_words = File_lower[i].split()
                        #checks to see if the word appears more than once in the line
			for j in range(0, len(temp_words)):
				if word == temp_words[j]:
					count+=1
                        #prints the output depending on if the word appears once, more than once or not at all
			if count == 1:
				num_spaces = long_word_len + 1 - len(word)
				space = " "			
				print(word.upper()+(num_spaces*space), File[i]+" ("+str(i+1)+")")
			elif count > 1:
				num_spaces = long_word_len + 1 - len(word)
				space = " "
				print(word.upper()+(num_spaces*space), File[i]+" ("+str(i+1)+"*)")
				
		
if __name__ == "__main__":
	main()
