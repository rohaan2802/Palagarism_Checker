#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

const int MAX_WORDS = 1000;
const int MAX_LINE_LENGTH = 1000;
char text_read[10][MAX_WORDS];
int unique_word_count=0;
char unique_words[10][100];
int frequency[100][100];
double simil[100][100];


class Input_File
{
private:
	char stop_words[MAX_WORDS][MAX_WORDS];
	char doc_path[10][100];
	int NumberOfDocument;
	int stop_word_count;
public:
	Input_File()
	{
		NumberOfDocument = 0;
		stop_word_count = 0;
	}

	void set_stopWords(const char* str)
	{
		int j = 0;
		stop_word_count = 0;
		for (int i = 12; str[i] != '\0'; i++) {
			if (str[i] != ' ') {
				stop_words[stop_word_count][j++] = str[i];
			}
			else {
				stop_words[stop_word_count][j] = '\0';
				stop_word_count++;
				j = 0;
			}
		}
		stop_words[stop_word_count++][j] = '\0';
	}
	const char* get_stopWords(int x)
	{
		if (x >= 0 && x < stop_word_count)
		{
			return stop_words[x];
		}
		return nullptr;
		

	}
	int get_stopWords_count()
	{
		return stop_word_count;
	}
	void setNumberOfDocument( int x)
	{
		NumberOfDocument = x;
	}
	int getNumberOfDocument()
	{
		return NumberOfDocument;
	}
	void set_doc_path(const char* str,int ind)
	{
		int i = 0;
		while (str[i] != '\0')
		{
			doc_path[ind][i] = str[i];
			i++;
		}
		doc_path[ind][i] = '\0';
	}
	const char* get_doc_path(int index) 
	{
		if (index >= 0 && index < NumberOfDocument)
		{
			return doc_path[index];
		}
		return nullptr;
	}
	
};

Input_File f1;

void readInput(const char* pathofInputFile) 
{
	ifstream file;
	char array[MAX_WORDS];
	file.open(pathofInputFile);
	file.getline(array, MAX_WORDS);
	f1.set_stopWords(array);
	file.getline(array, MAX_WORDS);
	int num_document = atoi(array);
	f1.setNumberOfDocument(num_document);
	for (int i = 0; i < num_document; i++) 
	{
		file.getline(array, MAX_WORDS);
		f1.set_doc_path(array, i);
	}
	file.close();
	for (int i = 0; i < num_document; i++) 
	{
		const char* file_name = f1.get_doc_path(i);
		ifstream file1(file_name);
		int index = 0;
		bool leading_space = true;
		char c;

		while (file1.get(c)) 
		{
			if (leading_space && (c == ' ' || c == '\t' || c == '\n')) 
			{
				continue;
			}

			leading_space = false;

			if (c == '\n' || c == ' ' || c == '\t')
			{
				if (index == 0 || text_read[i][index - 1] != ' ')
				{
					text_read[i][index++] = ' ';
				}
			}
			else 
			{
				text_read[i][index++] = c;
			}
		}
		text_read[i][index] = '\0';  
		file1.close();
	}
}













char* getText(int documentNumber) {
	
	return text_read[documentNumber -1];
}



int getNumberOfDocument()
{
	return f1.getNumberOfDocument();
}





void removePunctuationMarks()
{
	char array[10][MAX_WORDS];

	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		int count = 0;
		bool flag = true;

		for (int j = 0; text_read[i][j] != '\0'; j++)
		{
			if ((text_read[i][j] >= 'a' && text_read[i][j] <= 'z') || (text_read[i][j] >= 'A' && text_read[i][j] <= 'Z'))
			{
				array[i][count++] = text_read[i][j];
				flag = false;
			}
			else if (text_read[i][j] == ' ' && flag == false)
			{
				if (count == 0 || array[i][count - 1] != ' ') {
					array[i][count++] = ' ';
				}
				flag = true;
			}
		}

		if (count > 0 && array[i][count - 1] == ' ')
		{
			count--;
		}

		array[i][count] = '\0';
	}

	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		strcpy(text_read[i], array[i]);
	}
}





void convertUpperToLowerCase()
{
	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		

		for (int j = 0; text_read[i][j] != '\0'; j++)
		{
			if ( (text_read[i][j] >= 65 && text_read[i][j] <= 90))
			{
				text_read[i][j] = text_read[i][j] + 32;
			}
			
		}
		
	}

}


bool isStopWord(const char* ch)
{
	for (int i = 0; i < f1.get_stopWords_count(); i++)
	{
		int j = 0;
		while (ch[j] != '\0' && f1.get_stopWords(i)[j] != '\0' && ch[j] == f1.get_stopWords(i)[j])
		{
			j++;
		}

		if (ch[j] == '\0' && f1.get_stopWords(i)[j] == '\0')
		{
			return true;
		}
	}

	return false;
}


void removeStopWords()
{
	char temp_array[MAX_WORDS][MAX_WORDS];  

	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		int temp_count = 0;
		int temp_count1 = 0;

		for (int j = 0; text_read[i][j] != '\0'; j++)
		{
			if (text_read[i][j] != ' ')  
			{
				temp_array[temp_count][temp_count1++] = text_read[i][j];
			}
			else  
			{
				temp_array[temp_count][temp_count1] = '\0';  
				temp_count++; 
				temp_count1 = 0;  
			}
		}
		temp_array[temp_count++][temp_count1] = '\0';
		

		int new_index = 0, word_index = 0;  

		




		for (int k = 0; k < temp_count; k++)
		{
			

			if (isStopWord(temp_array[k])==false)
			{
				int m = 0;
				while (temp_array[k][m] != '\0')
				{
					text_read[i][new_index++] = temp_array[k][m++];
				}
				text_read[i][new_index++] = ' ';  
			}
		}

		text_read[i][new_index-1] = '\0';
	
	}
}



void generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency)
{
	uniqueCount = 0;
	uniqueWords = new char* [MAX_WORDS];
	documentFrequency = new int* [f1.getNumberOfDocument()];

	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		documentFrequency[i] = new int[100]();
	}

	for (int i = 0; i < f1.getNumberOfDocument(); i++)
	{
		char* word = strtok(text_read[i], " ");  
		while (word != nullptr)
		{
			bool found = false;

			for (int j = 0; j < uniqueCount; j++)
			{
				if (strcmp(uniqueWords[j], word) == 0)  
				{
					found = true;
					documentFrequency[i][j]++;
					frequency[i][j] = documentFrequency[i][j];
					break;
				}
			}

			if (!found)
			{
				uniqueWords[uniqueCount] = new char[strlen(word) + 1];
				strcpy(uniqueWords[uniqueCount], word);
				strcpy(unique_words[uniqueCount], uniqueWords[uniqueCount]);

				frequency[i][uniqueCount] = documentFrequency[i][uniqueCount] = 1;
				uniqueCount++;  
			}

			word = strtok(nullptr, " ");  
		}
	}

	unique_word_count = uniqueCount;
	

}






int getFrequency(char* word, int documentNum)   
{
	
	for (int i = 0; i < unique_word_count; i++)
	{
		if (strcmp(unique_words[i], word) == 0)
		{
			return frequency[documentNum-1][i];
		}
	}
	return -1;  
}






void calculateAllCosineSimilarities(double**& similarities, int** documentFrequency)
{
	int numDocuments = f1.getNumberOfDocument();

	similarities = new double* [numDocuments];
	for (int i = 0; i < numDocuments; i++)
	{
		similarities[i] = new double[numDocuments];
	}

	double* magnitude = new double[numDocuments];
	for (int i = 0; i < numDocuments; i++)
	{
		double sum = 0.0;
		for (int j = 0; j < unique_word_count; j++)
		{
			sum += pow(documentFrequency[i][j], 2);
		}
		magnitude[i] = sqrt(sum);
	}

	for (int i = 0; i < numDocuments; i++)
	{
		for (int j = 0; j < numDocuments; j++)
		{
			if (i == j)
			{
				similarities[i][j] = 1.0;  
				simil[i][j] = similarities[i][j];
			}
			else
			{
				double dotProduct = 0.0;
				for (int k = 0; k < unique_word_count; k++)
				{
					dotProduct += documentFrequency[i][k] * documentFrequency[j][k];
				}
				similarities[i][j] = dotProduct / (magnitude[i] * magnitude[j]);


				simil[i][j] = similarities[i][j];
			}
		}
	}

}




double similarityIn(int documentNum1, int documentNum2)
{
	int numDocuments = f1.getNumberOfDocument();

	if (documentNum1 < 1 || documentNum1 > numDocuments || documentNum2 < 1 || documentNum2 > numDocuments)
	{
		return 0;
	}

	int doc1 = documentNum1 - 1;
	int doc2 = documentNum2 - 1;

	return simil[doc1][doc2] * 100;
}




