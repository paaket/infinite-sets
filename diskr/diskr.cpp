#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

enum OPERATION {
	more, more_eq, les, less_eq, div_ost, natur, cel, otr
};

struct arguments {
	int first;
	int second;
};

struct unlimited {
	OPERATION oper;
	double value;
	arguments arg;
};

struct set {
	vector<unlimited> conditions;
};

vector<vector<double>> matrix;
vector<set> condition_matrix;

void matrix_generate();
void print_matrix();
void max_elem();
void min_elem();
void operations();
void clear_console();

int main() {
	setlocale(LC_ALL, "ru");

	matrix_generate();
	print_matrix();
	operations();

	return 0;
}

void clear_console() {
	std::cout << "\033[2J\033[H" << std::flush;
}

void unlim_cin(string str) {
	vector<unlimited> temp_set;
	bool top_unlim = 0, bottom_unlim = 0, div_unlim = 0;
	while (str != "#") {
		if (str.size() >= 3 and str[0] == 'x' and (str[1] == '>' or str[1] == '<' or str[1] == '%')) {
			if (str[1] == '>' and str[2] != '=') {
				if (bottom_unlim) {
					cout << "нижняя граница уже задана\n";
				}
				else {
					temp_set.push_back({ more, stod(str.substr(2)) });
					bottom_unlim = 1;
				}
			}
			if (str[1] == '<' and str[2] != '=') {
				if (top_unlim) {
					cout << "верхняя граница уже задана\n";
				}
				else {
					temp_set.push_back({ les, stod(str.substr(2)) });
					top_unlim = 1;
				}
			}
			if (str[1] == '>' and str[2] == '=') {
				if (bottom_unlim) {
					cout << "нижняя граница уже задана\n";
				}
				else {
					temp_set.push_back({ more_eq, stod(str.substr(3)) });
					bottom_unlim = 1;
				}
			}
			if (str[1] == '<' and str[2] == '=') {
				if (top_unlim) {
					cout << "верхняя граница уже задана\n";
				}
				else {
					temp_set.push_back({ less_eq, stod(str.substr(3)) });
					top_unlim = 1;
				}
			}
			if (str[1] == '%') {
				int div_first, div_second;
				if (div_unlim) {
					cout << "остаток от деления уже задан\n";
				}
				else {
					div_first = std::stoi(str.substr(str.find('%') + 1, str.find('=') - str.find('%') - 1));
					div_second = std::stoi(str.substr(str.find('=') + 1));
					temp_set.push_back({ div_ost, 0, { div_first, div_second } });
					div_unlim = 1;
				}
			}
			cin >> str;
		}
		else {
			cout << "некорректное условие\n";
			cin >> str;
		}
	}
	condition_matrix.push_back({ temp_set });
}

void matrix_generate() {
	cout << "==== СОЗДАНИЕ МНОЖЕСТВ ====\n";
	int var1, var2, arr_count = 0;
	bool flag = 0;
	string temp;

	while (!flag) {
		vector<double> arr;
		vector<unlimited> temp_set;
		bool top_unlim = 0, bottom_unlim = 0, div_unlim = 0;
		string str, in_file_name, line;
		ifstream file;

		cout << "\nновое множество: \n1 - конечное\n2 - бесконечное\n3 - множество натуральных чисел\n4 - множнство целых чисел\n5 - множество отрицательных чисел\n6 - импорт из файла\n0 - не добавлять множество" << endl;
		cin >> var1;
		while (var1 < 0 or var1 > 6 or cin.fail()) {
			cout << "некорректный вариант" << endl;
			cin.clear();
			cin.ignore(100, '\n');
			cin >> var1;
		}
		cin.ignore(100, '\n');
		switch (var1) {
		case 1:
			cout << "способ задания множества:\n1 - вручную\n2 - интервалом" << endl;
			cin >> var2;
			while (var2 < 1 or var2 > 2 or cin.fail()) {
				cout << "некорректный вариант" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				cin >> var2;
			}
			cin.ignore(100, '\n');
			switch (var2) {
			case 1:
				cout << "введите элементы множества через пробел, \"#\" - остановка ввода:\n";
				while (cin >> temp and temp != "#") {
					arr.push_back(stod(temp));
				}
				if (cin.fail()) {
					cin.clear();
					cin.ignore(100, '\n');
				}
				if (arr.size() != 0) {
					matrix.push_back(arr);
					arr_count++;
					cin.ignore(100, '\n');
				}
				break;
			case 2:
				double top, bottom;
				bool top_get = 0, bottom_get = 0;

				while (!top_get or !bottom_get) {
					top_get = 0, bottom_get = 0;
					cout << "введите 2 числа - сначала нижнюю, затем верхнюю границу" << endl;
					cout << "нижняя: ";
					cin >> bottom;
					if (cin.fail()) {
						cout << "некорректное значение" << endl;
						cin.ignore(100, '\n');
						cin.clear();
					}
					else {
						bottom_get = 1;
					}
					cin.ignore(100, '\n');

					cout << "верхняя: ";
					cin >> top;
					if (cin.fail()) {
						cout << "некорректное значение" << endl;
						cin.ignore(100, '\n');
						cin.clear();
					}
					else {
						top_get = 1;
					}
					cin.ignore(100, '\n');

					if (top < bottom and top_get and bottom_get) {
						cout << "нижняя граница не может быть больше верхней" << endl;
						top_get = 0;
						bottom_get = 0;
					}
				}
				condition_matrix.push_back({
					{
						{more_eq, bottom},
						{less_eq, top}
					}
					});
				arr_count++;
				break;
			}
			break;
		case 2:
			cout << "добавьте условия множества, пример: x>=0, x%2=0; через enter, \"#\" - остановка ввода:\n";
			cin >> str;
			unlim_cin(str);
			arr_count++;
			break;
		case 3:
			condition_matrix.push_back({
					{
						{natur, 0}
					}
				});
			cout << "\nмножество добавлено\n";
			arr_count++;
			break;
		case 4:
			condition_matrix.push_back({
					{
						{cel, 0}
					}
				});
			cout << "\nмножество добавлено\n";
			arr_count++;
			break;
		case 5:
			condition_matrix.push_back({
					{
						{otr, 0}
					}
				});
			cout << "\nмножество добавлено\n";
			arr_count++;
			break;
		case 6:
			arr.clear(), temp_set.clear();
			cout << "введите имя файла для импорта: ";
			cin >> in_file_name;
			file.open(in_file_name);
			if (file.is_open()) {
				while (getline(file, line)) {
					if (line[0] == 'c' and line[1] == 'o' and line[2] == 'n') {
						getline(file, line);
						while (line != "#") {
							arr.push_back(stod(line));
							getline(file, line);
						}
						matrix.push_back(arr);
						arr.clear();
						arr_count++;
					}
					if (line[0] == 'u' and line[1] == 'n' and line[2] == 'l') {
						temp_set.clear();
						getline(file, line);
						while (line != "#") {
							if (line[1] == '>' and line[2] != '=') {
								temp_set.push_back({ more, stod(line.substr(2)) });
							}
							if (line[1] == '<' and line[2] != '=') {
								temp_set.push_back({ les, stod(line.substr(2)) });
							}
							if (line[1] == '>' and line[2] == '=') {
								temp_set.push_back({ more_eq, stod(line.substr(3)) });
							}
							if (line[1] == '<' and line[2] == '=') {
								temp_set.push_back({ less_eq, stod(line.substr(3)) });
							}
							if (line[1] == '%') {
								int div_first, div_second;
								div_first = std::stoi(line.substr(line.find('%') + 1, line.find('=') - line.find('%') - 1));
								div_second = std::stoi(line.substr(line.find('=') + 1));
								temp_set.push_back({ div_ost, 0, { div_first, div_second } });
							}
							if (line == "n") {
								temp_set.push_back({ natur });
							}
							if (line == "c") {
								temp_set.push_back({ cel });
							}
							if (line == "o") {
								temp_set.push_back({ otr });
							}
							getline(file, line);
						}
						arr_count++;
						condition_matrix.push_back({ temp_set });
					}
				}
			}
			else {
				cout << "ошибка открытия файла" << endl;
			}
			cout << "успешно" << endl;
			file.close();
			break;
		case 0:
			if (arr_count >= 2) {
				flag = 1;
			}
			else {
				cout << "добавьте как минимум 2 множества";
			}
			break;
		}
	}
}

map<int, string> sets_list;

void print_matrix() {
	clear_console();
	int print_count = 1;
	cout << "\n==== СПИСОК МНОЖЕСТВ ===\n\n";
	for (int i = 0; i < matrix.size(); i++) {
		cout << print_count << ": ";
		for (int j = 0; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
		sets_list.insert({ print_count, "c"});
		print_count++;
	}
	for (int i = 0; i < condition_matrix.size(); i++) {
		cout << print_count << ": ";
		for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
			if (condition_matrix[i].conditions[j].oper == 0) {
				cout << "x>" << condition_matrix[i].conditions[j].value;
			}
			if (condition_matrix[i].conditions[j].oper == 1) {
				cout << "x>=" << condition_matrix[i].conditions[j].value;
			}
			if (condition_matrix[i].conditions[j].oper == 2) {
				cout << "x<" << condition_matrix[i].conditions[j].value;
			}
			if (condition_matrix[i].conditions[j].oper == 3) {
				cout << "x<=" << condition_matrix[i].conditions[j].value;
			}
			if (condition_matrix[i].conditions[j].oper == 4) {
				cout << "x%" << condition_matrix[i].conditions[j].arg.first <<
					"=" << condition_matrix[i].conditions[j].arg.second;
			}
			if (condition_matrix[i].conditions[j].oper == 5) {
				cout << "натуральные числа";
			}
			if (condition_matrix[i].conditions[j].oper == 6) {
				cout << "целые числа";
			}
			if (condition_matrix[i].conditions[j].oper == 7) {
				cout << "отрицательные числа";
			}
			if (j == condition_matrix[i].conditions.size() - 1) {
				cout << endl;
				sets_list.insert({ print_count, "u" });
				print_count++;
			}
			else {
				cout << " and ";
			}
		}
	}
}

void max_elem() {
	if (condition_matrix.size() == 0) {
		double max = matrix[0][0];
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				if (matrix[i][j] > max)
					max = matrix[i][j];
			}
		}
		cout << "\nмаксимальный элемент: " << max << endl;
		return;
	}
	if (condition_matrix.size() != 0) {
		double max = 0, is_les_value = 0;
		int pr_pos = -1, more_pos = -1, more_eq_pos = -1, start = 0, max_count = 0;

		if (matrix.size() != 0) {
			for (int i = 0; i < matrix.size(); i++) {
				for (int j = 0; j < matrix[i].size(); j++) {
					if (max_count == 0) {
						max = matrix[i][j];
						max_count++;
					}
					else if (matrix[i][j] > max) {
						max = matrix[i][j];
						max_count++;
					}
				}
			}
		}
		for (int i = 0; i < condition_matrix.size(); i++) {
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions[j].oper == 5 or condition_matrix[i].conditions[j].oper == 6 or
					condition_matrix[i].conditions[j].oper == 7) {
					cout << "\nмаксимального элемента нет\n";
					return;
				}
				if (condition_matrix[i].conditions.size() == 1 and condition_matrix[i].conditions[j].oper == 3) {
					if (max_count == 0) {
						max = condition_matrix[i].conditions[j].value;
						max_count++;
					}
					else if (condition_matrix[i].conditions[j].value > max) {
						max = condition_matrix[i].conditions[j].value;
					}
				}
				if (condition_matrix[i].conditions.size() == 2) {
					if (condition_matrix[i].conditions[j].oper == 4)
						pr_pos = j;
					if (condition_matrix[i].conditions[j].oper == 3) {
						if (max_count == 0) {
							max = condition_matrix[i].conditions[j].value;
							max_count++;
						}
						else if (condition_matrix[i].conditions[j].value > max) {
							max = condition_matrix[i].conditions[j].value;
						}
					}
				}
				if (condition_matrix[i].conditions.size() == 3) {
					if (condition_matrix[i].conditions[j].oper == 4)
						pr_pos = j;
					if (condition_matrix[i].conditions[j].oper == 0)
						more_pos = j;
					if (condition_matrix[i].conditions[j].oper == 1)
						more_eq_pos = j;
				}
			}
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions.size() == 2 and (condition_matrix[i].conditions[j].oper == 2 or
					condition_matrix[i].conditions[j].oper == 3) and pr_pos != -1) {
					start = condition_matrix[i].conditions[j].value;
					while (start % condition_matrix[i].conditions[pr_pos].arg.first != condition_matrix[i].conditions[pr_pos].arg.second) {
						start--;
					}
					if (max_count == 0) {
						if (condition_matrix[i].conditions[j].oper == 2) {
							is_les_value = start;
						}
						max = start;
						max_count++;
					}
					else if (start > max) {
						if (condition_matrix[i].conditions[j].oper == 2) {
							is_les_value = start;
						}
						max = start;
						max_count++;
					}
				}
				if (condition_matrix[i].conditions.size() == 2 and (condition_matrix[i].conditions[j].oper == 2 or
					condition_matrix[i].conditions[j].oper == 3) and pr_pos == -1) {
					if (max_count == 0) {
						if (condition_matrix[i].conditions[j].oper == 2) {
							is_les_value = condition_matrix[i].conditions[j].value;
						}
						max = condition_matrix[i].conditions[j].value;
						max_count++;
					}
					else if (condition_matrix[i].conditions[j].value > max) {
						if (condition_matrix[i].conditions[j].oper == 2) {
							is_les_value = condition_matrix[i].conditions[j].value;
						}
						max = condition_matrix[i].conditions[j].value;
						max_count++;
					}
				}
				if (condition_matrix[i].conditions.size() == 3 and (condition_matrix[i].conditions[j].oper == 2 or
					condition_matrix[i].conditions[j].oper == 3)) {
					start = condition_matrix[i].conditions[j].oper == 2 ? (condition_matrix[i].conditions[j].value - 1) :
						condition_matrix[i].conditions[j].value;
					while (start % condition_matrix[i].conditions[pr_pos].arg.first != condition_matrix[i].conditions[pr_pos].arg.second) {
						start--;
					}
					if (more_pos != -1) {
						if (start > condition_matrix[i].conditions[more_pos].value) {
							if (max_count == 0) {
								if (condition_matrix[i].conditions[j].oper == 2) {
									is_les_value = condition_matrix[i].conditions[j].value;
								}
								max = start;
								max_count++;
							}
							else if (start > max) {
								if (condition_matrix[i].conditions[j].oper == 2) {
									is_les_value = condition_matrix[i].conditions[j].value;
								}
								max = start;
								max_count++;
							}
						}
					}
					if (more_eq_pos != -1) {
						if (start >= condition_matrix[i].conditions[more_eq_pos].value) {
							if (max_count == 0) {
								if (condition_matrix[i].conditions[j].oper == 2) {
									is_les_value = condition_matrix[i].conditions[j].value;
								}
								max = start;
								max_count++;
							}
							else if (start > max) {
								if (condition_matrix[i].conditions[j].oper == 2) {
									is_les_value = condition_matrix[i].conditions[j].value;
								}
								max = start;
								max_count++;
							}
						}
					}
				}
			}
			more_pos = -1, more_eq_pos = -1, pr_pos = -1;
		}
		if (max_count == 0) {
			cout << "\nмаксимального элемента нет\n";
			return;
		}
		if (max == is_les_value) {
			cout << "\nневозможно определить максимальный элемент\n";
			return;
		}
		cout << "\nмаксимальный элемент: " << max << endl;
	}
}

void min_elem() {
	if (condition_matrix.size() == 0) {
		double min = matrix[0][0];
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				if (matrix[i][j] < min)
					min = matrix[i][j];
			}
		}
		cout << "\nминимальный элемент: " << min << endl;
		return;
	}
	if (condition_matrix.size() != 0) {
		double min = 0, is_more_value = 0;
		int pr_pos = -1, les_pos = -1, les_eq_pos = -1, start = 0, min_count = 0;

		if (matrix.size() != 0) {
			min = matrix[0][0];
			for (int i = 0; i < matrix.size(); i++) {
				for (int j = 0; j < matrix[i].size(); j++) {
					if (min_count == 0) {
						min = matrix[i][j];
						min_count++;
					}
					else if (matrix[i][j] < min) {
						min = matrix[i][j];
						min_count++;
					}
				}
			}
		}
		for (int i = 0; i < condition_matrix.size(); i++) {
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions[j].oper == 5 and 1 < min) {
					if (min_count == 0) {
						min = 1;
						min_count++;
					}
					else {
						min = 1;
						min_count++;
					}
				}
				if (condition_matrix[i].conditions.size() == 1 and condition_matrix[i].conditions[j].oper == 1) {
					if (min_count == 0) {
						min = condition_matrix[i].conditions[j].value;
						min_count++;
					}
					else if (condition_matrix[i].conditions[j].value < min) {
						min = condition_matrix[i].conditions[j].value;
					}
				}
				if (condition_matrix[i].conditions.size() == 2) {
					if (condition_matrix[i].conditions[j].oper == 4)
						pr_pos = j;
					if (condition_matrix[i].conditions[j].oper == 1) {
						if (min_count == 0) {
							min = condition_matrix[i].conditions[j].value;
							min_count++;
						}
						else if (condition_matrix[i].conditions[j].value < min) {
							min = condition_matrix[i].conditions[j].value;
						}
					}
				}
				if (condition_matrix[i].conditions.size() == 3) {
					if (condition_matrix[i].conditions[j].oper == 4)
						pr_pos = j;
					if (condition_matrix[i].conditions[j].oper == 2)
						les_pos = j;
					if (condition_matrix[i].conditions[j].oper == 3)
						les_eq_pos = j;
				}
			}
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions.size() == 2 and (condition_matrix[i].conditions[j].oper == 0 or
					condition_matrix[i].conditions[j].oper == 1) and pr_pos != -1) {
					start = condition_matrix[i].conditions[j].value;
					while (start % condition_matrix[i].conditions[pr_pos].arg.first != condition_matrix[i].conditions[pr_pos].arg.second) {
						start++;
					}
					if (min_count == 0) {
						if (condition_matrix[i].conditions[j].oper == 0) {
							is_more_value = start;
						}
						min = start;
						min_count++;
					}
					else if (start < min) {
						if (condition_matrix[i].conditions[j].oper == 0) {
							is_more_value = start;
						}
						min = start;
						min_count++;
					}
				}
				if (condition_matrix[i].conditions.size() == 2 and (condition_matrix[i].conditions[j].oper == 0 or
					condition_matrix[i].conditions[j].oper == 1) and pr_pos == -1) {
					if (min_count == 0) {
						if (condition_matrix[i].conditions[j].oper == 0) {
							is_more_value = condition_matrix[i].conditions[j].value;
						}
						min = condition_matrix[i].conditions[j].value;
						min_count++;
					}
					else if (condition_matrix[i].conditions[j].value < min) {
						if (condition_matrix[i].conditions[j].oper == 0) {
							is_more_value = condition_matrix[i].conditions[j].value;
						}
						min = condition_matrix[i].conditions[j].value;
						min_count++;
					}
				}
				if (condition_matrix[i].conditions.size() == 3 and (condition_matrix[i].conditions[j].oper == 0 or
					condition_matrix[i].conditions[j].oper == 1)) {
					start = condition_matrix[i].conditions[j].value;
					while (start % condition_matrix[i].conditions[pr_pos].arg.first != condition_matrix[i].conditions[pr_pos].arg.second) {
						start++;
					}
					if (les_pos != -1) {
						if (start < condition_matrix[i].conditions[les_pos].value and start < min) {
							if (min_count == 0) {
								if (condition_matrix[i].conditions[j].oper == 0) {
									is_more_value = condition_matrix[i].conditions[j].value;
								}
								min = start;
								min_count++;
							}
							else if (start < min) {
								if (condition_matrix[i].conditions[j].oper == 0) {
									is_more_value = condition_matrix[i].conditions[j].value;
								}
								min = start;
								min_count++;
							}
						}
					}
					if (les_eq_pos != -1) {
						if (start <= condition_matrix[i].conditions[les_eq_pos].value and start < min) {
							if (min_count == 0) {
								if (condition_matrix[i].conditions[j].oper == 0) {
									is_more_value = condition_matrix[i].conditions[j].value;
								}
								min = start;
								min_count++;
							}
							else if (start < min) {
								if (condition_matrix[i].conditions[j].oper == 0) {
									is_more_value = condition_matrix[i].conditions[j].value;
								}
								min = start;
								min_count++;
							}
						}
					}
				}
			}
			les_pos = -1, les_eq_pos = -1, pr_pos = -1;
		}
		if (min_count == 0) {
			cout << "\nминимального элемента нет\n";
			return;
		}
		if (min == is_more_value) {
			cout << "\nневозможно определить минимальный элемент\n";
			return;
		}
		cout << "\nминимальный элемент: " << min << endl;
	}
}

void common_elements() {
	if (condition_matrix.size() == 0) {
		vector<double> common_elements;
		vector<double> temp_matrix = matrix[0];
		int is_common = 0;
		for (int k = 0; k < temp_matrix.size(); k++) {
			for (int i = 1; i < matrix.size(); i++) {
				for (int j = 0; j < matrix[i].size(); j++) {
					if (matrix[i][j] == temp_matrix[k]) {
						is_common += 1;
					}
				}
			}
			if (is_common == matrix.size() - 1) {
				common_elements.push_back(temp_matrix[k]);
			}
			is_common = 0;
		}
		cout << endl;
		for (int i = 0; i < common_elements.size(); i++) {
			cout << common_elements[i] << " ";
		}
		cout << endl;
	}
	if (condition_matrix.size() != 0 and matrix.size() != 0) {
		vector<double> common_elements;
		vector<double> temp_matrix = matrix[0];
		vector<double> elem;
		int count = 0;
		bool is_common = 1;
		for (int k = 0; k < temp_matrix.size(); k++) {
			for (int i = 1; i < matrix.size(); i++) {
				for (int j = 0; j < matrix[i].size(); j++) {
					if (matrix[i][j] == temp_matrix[k]) {
						count += 1;
					}
				}
			}
			if (count == matrix.size() - 1) {
				common_elements.push_back(temp_matrix[k]);
			}
			count = 0;
		}
		for (int k = 0; k < common_elements.size(); k++) {
			for (int i = 0; i < condition_matrix.size(); i++) {
				for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
					if (condition_matrix[i].conditions[j].oper == 0) {
						if (common_elements[k] <= condition_matrix[i].conditions[j].value) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 1) {
						if (common_elements[k] < condition_matrix[i].conditions[j].value) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 2) {
						if (common_elements[k] >= condition_matrix[i].conditions[j].value) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 3) {
						if (common_elements[k] > condition_matrix[i].conditions[j].value) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 4) {
						if (fmod(common_elements[k], condition_matrix[i].conditions[j].arg.first) != condition_matrix[i].conditions[j].arg.second) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 5) {
						if (common_elements[k] <= 0 or common_elements[k] != floor(common_elements[k])) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 6) {
						if (common_elements[k] != floor(common_elements[k])) {
							is_common = 0;
						}
					}
					if (condition_matrix[i].conditions[j].oper == 7) {
						if (common_elements[k] >= 0 or common_elements[k] != floor(common_elements[k])) {
							is_common = 0;
						}
					}
				}
			}
			if (is_common) {
				elem.push_back(common_elements[k]);
			}
			is_common = 1;
		}
		cout << endl;
		for (int i = 0; i < elem.size(); i++) {
			cout << elem[i] << " ";
		}
		cout << endl;
		
	}
	if (condition_matrix.size() != 0 and matrix.size() == 0) {
		double bottom_value = 1, top_value = -1;
		bool natur_true = 0, cel_true = 0, otr_true = 0;
		int bottom_count = 0, top_count = 0, bottom_oper_num = 0, top_oper_num = 0;
		multimap<double, double> pr_value;
		for (int i = 0; i < condition_matrix.size(); i++) {
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions[j].oper == 0) {
					if (bottom_count == 0) {
						bottom_value = condition_matrix[i].conditions[j].value;
						bottom_oper_num = 0;
						bottom_count++;
					}
					else if (condition_matrix[i].conditions[j].value >= bottom_value) {
						bottom_value = condition_matrix[i].conditions[j].value;
						bottom_oper_num = 0;
					}
				}
				if (condition_matrix[i].conditions[j].oper == 1) {
					if (bottom_count == 0) {
						bottom_value = condition_matrix[i].conditions[j].value;
						bottom_oper_num = 1;
						bottom_count++;
					}
					else if (condition_matrix[i].conditions[j].value >= bottom_value) {
						bottom_value = condition_matrix[i].conditions[j].value;
						bottom_oper_num = 1;
					}
				}
				if (condition_matrix[i].conditions[j].oper == 2) {
					if (top_count == 0) {
						top_value = condition_matrix[i].conditions[j].value;
						top_oper_num = 2;
						top_count++;
					}
					else if (condition_matrix[i].conditions[j].value <= top_value) {
						top_value = condition_matrix[i].conditions[j].value;
						top_oper_num = 2;
					}
				}
				if (condition_matrix[i].conditions[j].oper == 3) {
					if (top_count == 0) {
						top_value = condition_matrix[i].conditions[j].value;
						top_oper_num = 3;
						top_count++;
					}
					else if (condition_matrix[i].conditions[j].value <= top_value) {
						top_value = condition_matrix[i].conditions[j].value;
						top_oper_num = 3;
					}
				}
				if (condition_matrix[i].conditions[j].oper == 4) {
					pr_value.insert({ condition_matrix[i].conditions[j].arg.first, condition_matrix[i].conditions[j].arg.second });
				}
				if (condition_matrix[i].conditions[j].oper == 5) {
					natur_true = 1;
				}
				if (condition_matrix[i].conditions[j].oper == 6) {
					cel_true = 1;
				}
				if (condition_matrix[i].conditions[j].oper == 7) {
					otr_true = 1;
				}
			}
		}
		cout << endl;
		if (bottom_oper_num == 0 and bottom_count != 0) {
			cout << "x>" << bottom_value;
		}
		else if (bottom_count != 0){
			cout << "x>=" << bottom_value;
		}
		if (top_oper_num == 2 and top_count != 0) {
			if (bottom_count != 0)
				cout << " and ";
			cout << "x<" << top_value;
		}
		else if (top_count != 0){
			if (bottom_count != 0)
				cout << " and ";
			cout << "x<=" << top_value;
		}
		for (auto it = pr_value.begin(); it != pr_value.end(); it++) {
			cout << " and x%" << it->first << "=" << it->second;
		}
		if (natur_true and bottom_value > 0) {
			if (bottom_count != 0 or top_count != 0 or pr_value.size() != 0)
				cout << " and ";
			cout << "натуральные числа";
		}
		if (cel_true) {
			if (bottom_count != 0 or top_count != 0 or pr_value.size() != 0 or natur_true)
				cout << " and ";
			cout << "целые числа";
		}
		if (otr_true and top_value < 0) {
			if (bottom_count != 0 or top_count != 0 or pr_value.size() != 0 or natur_true or cel_true)
				cout << " and ";
			cout << "отрицательные числа";
		}
		cout << endl;
	}
}

void serialaze() {
	string out_file_name;
	cout << "введите имя файла для вывода: ";
	cin >> out_file_name;

	ofstream file(out_file_name);
	if (file.is_open()) {
		for (int i = 0; i < matrix.size(); i++) {
			file << "con" << endl;
			for (int j = 0; j < matrix[i].size(); j++) {
				file << matrix[i][j] << endl;
			}
			file << "#" << endl;
		}
		for (int i = 0; i < condition_matrix.size(); i++) {
			file << "unl" << endl;
			for (int j = 0; j < condition_matrix[i].conditions.size(); j++) {
				if (condition_matrix[i].conditions[j].oper == 0) {
					file << "x>" << condition_matrix[i].conditions[j].value;
				}
				if (condition_matrix[i].conditions[j].oper == 1) {
					file << "x>=" << condition_matrix[i].conditions[j].value;
				}
				if (condition_matrix[i].conditions[j].oper == 2) {
					file << "x<" << condition_matrix[i].conditions[j].value;
				}
				if (condition_matrix[i].conditions[j].oper == 3) {
					file << "x<=" << condition_matrix[i].conditions[j].value;
				}
				if (condition_matrix[i].conditions[j].oper == 4) {
					file << "x%" << condition_matrix[i].conditions[j].arg.first <<
						"=" << condition_matrix[i].conditions[j].arg.second;
				}
				if (condition_matrix[i].conditions[j].oper == 5) {
					file << "n";
				}
				if (condition_matrix[i].conditions[j].oper == 6) {
					file << "c";
				}
				if (condition_matrix[i].conditions[j].oper == 7) {
					file << "o";
				}
				if (j == condition_matrix[i].conditions.size() - 1) {
					file << "\n#\n";
				}
				else {
					file << endl;
				}
			}
		}
		cout << "успешно" << endl;
	}
	else {
		cout << "ошибка открытия" << endl;
	}
	file.close();
}

void elements_count() {
	double top, bottom;
	int num, count = 0;
	bool flag = 1, pr_true = 0;
	cout << "номер множества: ";
	cin >> num;
	cout << "введите верхнюю и нижнюю границу диапазона:" << endl;
	while (flag) {
		cout << "нижняя: ";
		cin >> bottom;
		while (bottom < 1) {
			cout << "некорреткное значение\n";
			cout << "нижняя: ";
			cin >> bottom;
		}
		cout << "верхняя: ";
		cin >> top;
		while (top < 1) {
			cout << "некорреткное значение\n";
			cout << "верхняя: ";
			cin >> top;
		}
		if (top == bottom) {
			cout << "границы не могут быть равны" << endl;
		}
		else {
			flag = 0;
		}
	}
	if (sets_list[num] == "c") {
		cout << endl;
		for (int i = bottom - 1; i <= top - 1; i++) {
			cout << matrix[num - 1][i] << " ";
		}
		cout << endl;
	}
	if (sets_list[num] == "u") {

	}
}

void count_belong() {
	int set_num = 0, is_belong = 0, count = 0;
	cout << "введите номер множества: ";
	cin >> set_num;
	cout << "введите число: ";
	cin >> count;
	if (sets_list[set_num] == "c") {
		for (int i = 0; i < matrix[set_num - 1].size(); i++) {
			if (matrix[set_num - 1][i] == count)
				is_belong++;
		}
		if (is_belong != 0) {
			cout << "\nчисло " << count << " принадлнжит множеству " << set_num << endl;
		}
		else {
			cout << "\nчисло " << count << " не принадлнжит множеству " << set_num << endl;
		}
	}
	else {
		for (int i = 0; i < condition_matrix[set_num - matrix.size() - 1].conditions.size(); i++) {
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 0) {
				if (count > condition_matrix[set_num - matrix.size() - 1].conditions[i].value)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 1) {
				if (count >= condition_matrix[set_num - matrix.size() - 1].conditions[i].value)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 2) {
				if (count < condition_matrix[set_num - matrix.size() - 1].conditions[i].value)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 3) {
				if (count <= condition_matrix[set_num - matrix.size() - 1].conditions[i].value)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 4) {
				if (count % condition_matrix[set_num - matrix.size() - 1].conditions[i].arg.first == 
					condition_matrix[set_num - matrix.size() - 1].conditions[i].arg.second)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 5) {
				if (count > 0 and count == floor(count))
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 6) {
				if (count == floor(count))
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (condition_matrix[set_num - matrix.size() - 1].conditions[i].oper == 7) {
				if (count < 0)
					is_belong = 1;
				else
					is_belong = 0;
			}
			if (is_belong == 0) {
				cout << "\nчисло " << count << " не принадлнжит множеству " << set_num << endl;
				return;
			}
		}
	}
	cout << "\nчисло " << count << " принадлнжит множеству " << set_num << endl;
}

void set_belong() {
	int var, num_con, num = 0, flag = 0;
	bool case_one_true = 0;
	string temp;
	vector<double> arr;
	cout << "\nвыбор конечного подмножества:\n1 - из существующих\n2 - создать новое\n";
	cin >> var;
	while (var != 1 and var != 2) {
		cout << "некорректный вариант" << endl;
		cin >> var;
	}
	switch (var) {
		case 1:
			cout << "введите номер подмножества: ";
			cin >> num_con;
			case_one_true = 1;
			break;
		case 2:
			cout << "введите элементы подмножества через пробел, \"#\" - остановка ввода:\n";
			while (cin >> temp and temp != "#") {
				arr.push_back(stod(temp));
			}
			if (cin.fail()) {
				cin.clear();
				cin.ignore(100, '\n');
			}
			break;
	}
	cout << "введите номер множества для проверки: ";
	cin >> num;
	if (sets_list[num] == "c") {
		if (case_one_true) {
			for (int i = 0; i < matrix[num_con - 1].size(); i++) {
				for (int j = 0; j < matrix[num - 1].size(); j++) {
					if (matrix[num_con - 1][i] == matrix[num - 1][j]) {
						flag++;
					}
				}
				if (flag == 0) {
					cout << "\nмножество не является подмножеством" << endl;
					return;
				}
				flag = 0;
			}
		}
		else {
			for (int i = 0; i < arr.size(); i++) {
				for (int j = 0; j < matrix[num - 1].size(); j++) {
					if (arr[i] == matrix[num - 1][j]) {
						flag++;
					}
				}
				if (flag == 0) {
					cout << "\nмножество не является подмножеством" << endl;
					return;
				}
				flag = 0;
			}
		}
		cout << "\nмножество является подмножеством" << endl;
	}
	else {
		if (case_one_true) {
			for (int i = 0; i < matrix[num_con - 1].size(); i++) {
				for (int j = 0; j < condition_matrix[num - matrix.size() - 1].conditions.size(); j++) {
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 0) {
						if (matrix[num_con - 1][i] > condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 1) {
						if (matrix[num_con - 1][i] >= condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 2) {
						if (matrix[num_con - 1][i] < condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 3) {
						if (matrix[num_con - 1][i] <= condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 4) {
						if ((int)matrix[num_con - 1][i] % condition_matrix[num - matrix.size() - 1].conditions[j].arg.first ==
							condition_matrix[num - matrix.size() - 1].conditions[j].arg.second)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 5) {
						if (matrix[num_con - 1][i] > 0 and matrix[num_con - 1][i] == floor(matrix[num_con - 1][i]))
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 6) {
						if (matrix[num_con - 1][i] == floor(matrix[num_con - 1][i]))
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 7) {
						if (matrix[num_con - 1][i] < 0)
							flag++;
					}
					if (flag == 0) {
						cout << "\nмножество не является подмножеством" << endl;
						return;
					}
					flag = 0;
				}
			}
		}
		else {
			for (int i = 0; i < arr.size(); i++) {
				for (int j = 0; j < condition_matrix[num - matrix.size() - 1].conditions.size(); j++) {
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 0) {
						if (arr[i] > condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 1) {
						if (arr[i] >= condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 2) {
						if (arr[i] < condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 3) {
						if (arr[i] <= condition_matrix[num - matrix.size() - 1].conditions[j].value)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 4) {
						if ((int)arr[i] % condition_matrix[num - matrix.size() - 1].conditions[j].arg.first ==
							condition_matrix[num - matrix.size() - 1].conditions[j].arg.second)
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 5) {
						if (arr[i] > 0 and arr[i] == floor(arr[i]))
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 6) {
						if (arr[i] == floor(arr[i]))
							flag++;
					}
					if (condition_matrix[num - matrix.size() - 1].conditions[j].oper == 7) {
						if (arr[i] < 0)
							flag++;
					}
					if (flag == 0) {
						cout << "\nмножество не является подмножеством" << endl;
						return;
					}
					flag = 0;
				}
			}
		}
		cout << "\nмножество является подмножеством" << endl;
	}
}

void set_difference() {
	int first_num, second_num, count = 0, flag = 0;
	vector<double> result;
	if (matrix.size() == 0) {
		cout << "\nнет добавленных конечных множеств\n";
		return;
	}
	cout << "введите номер первго множества: ";
	cin >> first_num;
	while (first_num > matrix.size()) {
		cout << "выберите конечное множество" << endl;
		cin >> first_num;
	}
	cout << "введите номер второго множества: ";
	cin >> second_num;
	if (sets_list[first_num] == "c" and sets_list[second_num] == "c") {
		for (int i = 0; i < matrix[first_num - 1].size(); i++) {
			for (int j = 0; j < matrix[second_num - 1].size(); j++) {
				if (matrix[first_num - 1][i] == matrix[second_num - 1][j]) {
					count++;
				}
			}
			if (count == 0) {
				result.push_back(matrix[first_num - 1][i]);
			}
			count = 0;
		}
	}
	if (sets_list[first_num] == "c" or sets_list[second_num] == "u") {
		for (int i = 0; i < matrix[first_num - 1].size(); i++) {
			for (int j = 0; j < condition_matrix[second_num - matrix.size() - 1].conditions.size(); j++) {
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 0) {
					if (matrix[first_num - 1][i] <= condition_matrix[second_num - matrix.size() - 1].conditions[j].value)
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 1) {
					if (matrix[first_num - 1][i] < condition_matrix[second_num - matrix.size() - 1].conditions[j].value)
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 2) {
					if (matrix[first_num - 1][i] >= condition_matrix[second_num - matrix.size() - 1].conditions[j].value)
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 3) {
					if (matrix[first_num - 1][i] > condition_matrix[second_num - matrix.size() - 1].conditions[j].value)
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 4) {
					if ((int)matrix[first_num - 1][i] % condition_matrix[second_num - matrix.size() - 1].conditions[j].arg.first !=
						condition_matrix[second_num - matrix.size() - 1].conditions[j].arg.second)
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 5) {
					if (matrix[first_num - 1][i] <= 0 or matrix[first_num - 1][i] != floor(matrix[first_num - 1][i]))
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 6) {
					if (matrix[first_num - 1][i] != floor(matrix[first_num - 1][i]))
						flag++;
				}
				if (condition_matrix[second_num - matrix.size() - 1].conditions[j].oper == 7) {
					if (matrix[first_num - 1][i] >= 0)
						flag++;
				}
				if (flag != 0) {
					result.push_back(matrix[first_num - 1][i]);
				}
				flag = 0;
			}
		}
	}
	cout << endl;
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
	result.clear();
}

void operations() {
	int var1;
	bool flag = 0;
	cout << "\n==== ДЕЙСТВИЯ НАД МНОЖЕСТВАМИ ====\n\n";
	cout << "1 - поиск максимального элемента\n2 - поиск минимального элемента\n3 - пересечение множеств\n4 - экспорт множеств в файл\n5 - подсчёт элементов в диапазоне\n6 - принадлежит ли число множеству\n7 - является ли множество подмножеством\n8 - разность множеств\n0 - выход";
	while (!flag)
	{
		cout << "\nвариант: ";
		cin >> var1;
		switch (var1) {
		case 1:
			max_elem();
			break;
		case 2:
			min_elem();
			break;
		case 3:
			common_elements();
			break;
		case 4:
			serialaze();
			break;
		case 5:
			elements_count();
			break;
		case 6:
			count_belong();
			break;
		case 7:
			set_belong();
			break;
		case 8:
			set_difference();
			break;
		case 0:
			flag = 1;
			break;
		}
	}
}