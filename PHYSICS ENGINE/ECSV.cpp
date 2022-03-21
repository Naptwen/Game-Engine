#include "ECSV.h"

void CSV_to_vector(std::string file_name, void* rec_data, char TYPE)
{
	std::ifstream csv_file;
	std::string str;
	std::string token;
	std::string delimiter = ",";
	size_t pos = 0;
	file_name = file_name + ".csv";
	csv_file.open(file_name, std::ios::in);
	while (!csv_file.eof())
	{
		getline(csv_file, str);

		// delimiters present in str[]. 

		while ((pos = str.find(",")) != std::string::npos)
		{
			token = str.substr(0, pos);
			switch (TYPE)
			{
			case CHARCSV:
			{
				char value = stoi(token);
				(*(std::vector<char>*)rec_data).push_back(value);
				break;
			}
			case INTCSV:
			{
				int value = stoi(token);
				(*(std::vector<int>*)rec_data).push_back(value);
				break;
			}
			case FLOATCSV:
			{
				float value = stof(token);
				(*(std::vector<float>*)rec_data).push_back(value);
				break;
			}
			case DOUBLECSV:
			{
				double value = stod(token);
				(*(std::vector<double>*)rec_data).push_back(value);
				break;
			}
			case STRINGCSV:
			{
				(*(std::vector<std::string>*)rec_data).push_back(token);
			}
			default:
			{
				char value = stoi(token);
				(*(std::vector<char>*)rec_data).push_back(value);
				break;
			}
			}

			str.erase(0, pos + delimiter.length());
		}
	}
	csv_file.close();
}
void vector_to_CSV(std::vector<double>& decitions, std::string name, char MODE)
{
	name = name + ".csv";
	std::ofstream write(name, std::ios::app);
	int range = (int)decitions.size();
	if (write.is_open())
	{
		for (int i = 0; i < range; i++)
			write << decitions[i] << ", \n";
	}
	write.close();
};