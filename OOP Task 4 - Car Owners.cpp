/*
Задача 4. Напишете програма, която ще представлява база от данни за
притежатели на коли, съхранявайки следните данни:
● Име на притежателя (не по дълго от 23 символа)
● Униканлен идентификационен номер на колата
● Регистрационен номер на колата (неотрицателно четирицифрено число)
Колите, които могат да бъдат притежавани от хората, са:
име идентификационен номер мощност
Lambordgini Murcielago 0 670
Mercedes-AMG 1 503
Pagani Zonda R 2 740
Bugatti Veyron 3 1020
Вашата програма трябва да поддържа следните функции:
● Добавяне на нов запис за притежател на кола с име, регистрационен номер на
колата и идентификационен номер на колата. Ако в програмата съществува
запис за кола със същия регистрационен номер, извеждате съобщение за
грешка и не добавяте записа.
● Генериране на текстов файл с име "car-report.txt" и следната информация:
o Най-популярна кола - най-много хора я притежават.
o Средна мощност на всички притежавани коли.
o По един ред за всеки притежател с името и общата мощност на
притежаваните от него коли.
● Генериране на бинарен файл с име "db-save.dat", в който седят записи за
всички притежатели на коли. Ако файлът вече съществува, се презаписва.
● При пускане, да зарежда притежатели на коли от бинарен файл с име
"db-save.dat", ако такъв не съществува все едно е бил празен.

Забележки:
● Един човек, може да притежава повече от една кола - за програмата един и
същи човек е такъв, който присъства няколко пъти с еднакво име.
● Не качвайте бинарни файлове генерирани от вашата среда за разработка.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Car
{
public:
    Car(std::string b, size_t u, size_t hp) : m_brand(b), m_uniqueIdNumber(u), m_horsePower(hp) {
        if (u < 0)
            throw "Invalid value!";

        if (hp < 0 || hp > 1500)
            throw "Invalid value!";
    };

    std::string m_brand;
    size_t m_uniqueIdNumber = 0;
    size_t m_horsePower = 0;

};


class CarOwner
{
public:

    CarOwner(const char* initName, size_t i, size_t l ) :  m_idNumber(i), m_licensePlate(l)
    {
        strncpy_s(m_name, sizeof(m_name), initName, _TRUNCATE);;
        m_name[sizeof(m_name) - 1] = '\0';

        if (i < 0)
            throw "Invalid ID number!\n";
        if (l < 1000 || l > 9999) {
            throw "License plate should have only 4 numbers (e.g 1234)!\n";
        }
    }

    static const int MAX_NAME_SIZE = 23;
    char m_name[MAX_NAME_SIZE+1];
    size_t m_licensePlate = 0;
    size_t m_idNumber = 0;
};

int Menu();
bool CreateCar(std::vector<Car>& cars, const std::string& brand, size_t uniqueIDnumber, size_t horsePower);
bool AddOwner(std::vector<CarOwner>& carOwners, const char name[], const size_t& idNumber, const size_t& licensePlate);
void AverageHorsePower(const std::vector<CarOwner>& carOwners, const std::vector<Car>& cars, std::ostream& file);
void ListOfCars(const std::vector<Car>& cars);


int main()
{   
    //Този вектор ще съдържа създадените коли
    std::vector<Car> cars;
    //Този вектор ще съдържа създадените собственици
    std::vector<CarOwner> carOwners;



    bool running = true;
    while (running) {
        //Тази променлива ще бъде използвана за контролиране на програмата
        size_t option = Menu();
        switch (option) {
        case 1:
        {
            std::string brand;
            size_t uniqueIDnumber = 0;
            size_t horsePower = 0;

            std::cin >> brand;
            std::cin >> uniqueIDnumber;
            std::cin >> horsePower;
            if (CreateCar(cars, brand, uniqueIDnumber, horsePower))
                std::cout << "Car was created!\n";
            else {
                std::cout << "Car was not created!\n";
            }
            break;
        }

        case 2:
        {
            //Това ще бъде използвано за създаване на собствениците
            char name[23];
            size_t idNumber = 0;
            size_t licensePlate = 0;
            std::cout << "Enter the name of the car owner: ";
            std::cin >> name;
            std::cin.ignore();
            ListOfCars(cars);
            std::cout << "Enter the Unique ID number of the car that you want to purchase: \n\n";
            std::cin >> idNumber;
            std::cout << "Enter the number of the license plate under which the car will be registered (e.g 1234): ";
            std::cin >> licensePlate;
            if (AddOwner(carOwners, name, idNumber, licensePlate)) {
                std::cout << "Car owner was created!\n";
            }
            else {
                std::cout << "Car owner was not created!\n";
            }
        }
            break;

        case 3:
        {
            std::fstream reportFile("car-report.txt", std::ios::out);
            if (!reportFile) {
                std::cout << "\nError! File was not created!\n";
                return 1;
            }
            else {
                AverageHorsePower(carOwners, cars, reportFile);
                reportFile.close();
            }
            break;
        }

        case 4:
            break;
        case 5:
            running = false;
            break;
        default:
            break;
        }
    }

    
    return 0;
}

//Тази функция ще показва менюто на програмата
int Menu() {
    int opt = 0;

    do {
        std::cout << "Choose an option: \n";
        std::cout << "1. Create a car\n";
        std::cout << "2. Create a new car owner\n";
        std::cout << "3. Car report\n";
        std::cout << "4. Generate binary file\n";
        std::cout << "5. Exit\n";
        std::cin >> opt;
    } while (opt < 1 || opt > 4);

    return opt;
}

//Тази фукнция ще бъде използвана за създаване на нови собственици на коли
bool AddOwner(std::vector<CarOwner>& carOwners, const char name[], const size_t& idNumber, const size_t& licensePlate) {
    

    //Тази част ще проверява дали вече съществува кола със зададения регистрационен номер
    for (size_t i = 0; i < carOwners.size(); i++) {
        if (licensePlate == carOwners[i].m_licensePlate) {
            std::cout << "License plate already exist! Entry was not created!\n";
            return false;
        }
    }

    try {
        CarOwner carOwner(name, idNumber, licensePlate);
        carOwners.push_back(carOwner);
    }
    catch (const char* txt) {
        std::cout << "\nException: " << txt << "\n";
        return false;
    }
    return true;
}


//Тази функция ще създава колите
bool CreateCar(std::vector<Car>& cars, const std::string& brand, size_t uniqueIDnumber, size_t horsePower) {

    try {
        Car car(brand, uniqueIDnumber, horsePower);
        cars.push_back(car);
    }
    catch (const char* txt) {
        std::cout << "\nException: " << txt << "\n";
        return false;
    }

    return true;
}


//Тази фукнция ще се използва да може да се генерира car-report.txt
void AverageHorsePower(const std::vector<CarOwner>& carOwners, const std::vector<Car>& cars, std::ostream& file) {

    double totalHorsePower = 0;
    double averageHorsePower = 0.0;


    //Тази част ще калулира средната мощност на всички коли
    for (size_t i = 0; i < carOwners.size(); i++) {
        if (carOwners[i].m_idNumber < cars.size()) {
            totalHorsePower += cars[carOwners[i].m_idNumber].m_horsePower;
        }
    }
    averageHorsePower = totalHorsePower / carOwners.size();

    file << "The average HorsePower of all cars is : ";
    file << averageHorsePower;
    
}

//Това ше изкарва всички налични коли за да може потребителя да си избере
void ListOfCars(const std::vector<Car>& cars) {
    std::cout << "\n                  LIST OF CARS                     \n";
    std::cout << "Brand           HorsePowers        Unique ID Number\n";
    std::cout << "---------------------------------------------------\n";
    for (size_t i = 0; i < cars.size(); ++i) {
        std::cout << cars[i].m_brand << "             " << cars[i].m_horsePower << "                      " << cars[i].m_uniqueIdNumber << "\n";
    }
}
