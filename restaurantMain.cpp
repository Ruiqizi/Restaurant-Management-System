#include<iostream>
#include<string>
#include <limits>
#include <algorithm>
#include<fstream>
#include<map>
#include <iterator>
#include<sstream>
#include<vector>
#include<exception>
using namespace std;

float sum;

#pragma region
float load_sum() {
    std::ofstream tempFile("Sum.txt", ios::out | ios::app);
    if (!tempFile.is_open()) {
        std::cout << "Fail to create the file." << std::endl;
        exit(EXIT_FAILURE);
    }
    tempFile.close();

    ifstream sumfile;
    sumfile.open("Sum.txt", ios::in);
    if (!sumfile.is_open()) {
        cout << "Fail to open the file." << endl;
        exit(EXIT_FAILURE);
    }
    else {
        if (sumfile.peek() == ifstream::traits_type::eof()) {
            sum = 0.0;
        }
        else {
            sumfile >> sum;
        }
        sumfile.close();
        return sum;
    }
}


void update_sum() {
    ofstream sumfile;
    sumfile.open("Sum.txt", ios::out);
    if (!sumfile.is_open()) {
        cout << "Fail to open the file." << endl;
        exit(EXIT_FAILURE);
    }
    else {
        sumfile << sum;
        sumfile.close();
    }

}
//**********************************************************************  material
// This is the class for the material
class Material {
private:
    int m_id;
    float m_cost;
    string m_name;
    //Material* m_next;
public:
    Material(int id = 0, float cost = 0.0, string name = "") :m_id(id), m_cost(cost), m_name(name) {}
    int Get_m_id() {
        return m_id;
    }
    float Get_m_cost()const {
        return m_cost;
    }
    string Get_m_name()const {
        return m_name;
    }
    void fix_m_id(int i) {
        m_id = i;
    }
    void m_edit(int id, float cost, string name) {
        m_id = id;
        m_cost = cost;
        m_name = name;
    }
    //Insertion operator overloaded, used to output Material objects to ostream.
    friend std::ostream& operator<<(std::ostream& os, const Material& mat) {
        os << mat.m_id << " " << mat.m_cost << " " << mat.m_name;
        return os;
    }
    // Extraction operator overload, used to read Material objects from istream.
    friend std::istream& operator>>(std::istream& is, Material& mat) {
        is >> mat.m_id >> mat.m_cost >> mat.m_name;
        return is;
    }

    friend void m_edit(Material& material);
    friend int m_add(Material& material);
    ~Material() {}
};

map<int, Material> materials;
int GetMaxId_1(int type) { //The function which can get the maximum number of the id of the existent classes
    if (type == 0) {
        if (!materials.empty()) {
            auto it = --materials.end();
            return it->first;
        }
    }
    // If there is no id or the parameter is the wrong type(like 3), return a error situation
    return -1;
}


bool View_total_materials() {
    if (materials.empty()) {
        cout << "The materials haven't been edited." << endl;
        return false;
    }
    else {
        for (const auto& pair : materials) {
            cout << pair.first + 1 << ". " << pair.second.Get_m_name() << " ------------ " << pair.second.Get_m_cost() << " yuan" << endl;
        }
        cout << "\n";
        return true;
    }
}

int m_add(Material& new_material) {
    View_total_materials();
    int new_id = GetMaxId_1(0) + 2;
    float new_cost;
    string new_name;
    cout << "Defaults the id of your add material is " << new_id << endl;

    cout << "Please enter the name of your new material:";
    cin >> new_name;

    cout << "Please enter the cost of your new material:";
    cin >> new_cost;
    new_material.m_id = new_id;
    new_material.m_name = new_name;
    new_material.m_cost = new_cost;
    cout << "The new material " << new_name << "has been add successfully";
    return new_id;
}

int m_delete(Material& delete_material) {
    View_total_materials();
    cout << "Please enter the id of the dish that you want to delete" << endl;
m0:
    int delete_materialid;
    cin >> delete_materialid;
    //judgemental input
    while (!delete_materialid) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (delete_materialid < 0) {
        cout << "Please enter a number which is equal to 0 or larger than 0." << endl;
        goto m0;
    }

    //delete
    if (materials.find(delete_materialid) != materials.end()) { // If the dish corresponding to this id is found
        materials.erase(delete_materialid);
    }
    else {
        std::cout << "Material with id " << delete_materialid << " not found." << std::endl;
    }
    return delete_materialid;
}


void m_edit(Material& material) {
    int type = GetMaxId_1(0);
    if (type == -1) {
        cout << "There no material in the library" << endl;
    }
    else {
        cout << "The maxmum id(m) of the existing materials is " << type + 1 << endl;
        cout << "So you should enter the id between 0 to " << type + 1 << endl;
    }
    cout << "Please edit the id of the material" << endl;

m5:int id;
    while (!(cin >> id)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (id < 0) {
        cout << "Please enter a number which is equal to 0 or larger than 0." << endl;
        goto m5;
    }
    material.m_id = id;

    cout << "Please give the material a new name" << endl;
    string name;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (name.empty()) {
        cout << "Please edit the name of the material in a correct way:" << endl;
        cin.clear();
        getline(cin, name);
    }
    material.m_name = name;

    cout << "Please edit the cost of the material" << endl;
    float cost;
    while (!(cin >> cost)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    material.m_cost = cost;

    cout << "You have operated successfully" << endl;
}

//**********************************************************************   dish
//This is the class for the dish
class Dish {
private:
    int d_id;
    float d_price;
    float d_interest;
    string d_name;
    vector<vector<int>> material_table;
    int material_kind;



public:
    Dish(int id = 0, float price = 0.0, float interest = 0.0, string name = "", int n = GetMaxId_1(0)) :d_id(id), d_price(price), d_interest(interest), d_name(name), material_kind(n + 1) {
        if (material_kind > 0) {
            material_table.resize(2, vector<int>(material_kind, -1));
        }
    }
    // There are other functions
    int Get_d_id() {
        return d_id;
    }
    float Get_d_price()const {
        return d_price;
    }
    float Get_d_interest() const {
        return d_interest;
    }
    string Get_d_name() const {
        return d_name;
    }
    int Get_material_kind() const {
        return material_kind;
    }
    void fix_id(int i) {
        d_id = i;
    }
    vector<vector<int>>& Get_material_table() { return material_table; }

    friend void d_edit(Dish& dish);
    friend void View_Material_list(Dish& dish);
    // Converting a two-dimensional array of pointers to a string

  // Output Operator Overloading for the Dish Class
    friend ostream& operator<<(ostream& os, const Dish& dish) {
        os << dish.d_id << " " << dish.d_price << " "
            << dish.d_interest << " " << dish.d_name << " ";
        // Serialise material_table, with elements in each column determined by GetMaxId_1(0) + 1
        for (int i = 0; i < dish.material_table[0].size(); i++) {
            os << dish.material_table[0][i] << " " << dish.material_table[1][i];
            if (i < dish.material_table[0].size() - 1) os << "|"; // column separator
        }
        return os;
    }


    // Input Operator Overloading for the Dish Class
    friend istream& operator>>(istream& is, Dish& dish) {
        is >> dish.d_id >> dish.d_price >>
            dish.d_interest >> dish.d_name;

        int material_count = GetMaxId_1(0) + 1; // columns
        dish.material_table.resize(2, vector<int>(material_count, -1)); // initialisation material_table

        string item;
        // Separate the values in each column with a vertical bar '|'.
        for (int i = 0; i < material_count && getline(is, item, '|'); i++) {
            istringstream iss(item);
            iss >> dish.material_table[0][i] >> dish.material_table[1][i];
        }
        return is;
    }


    ~Dish() {}
};

map<int, Dish> dishes;
int GetMaxId_2(int type) { //The function which can get the maximum number of the id of the existent classes
    if (type == 1) {
        if (!dishes.empty()) {
            auto it = --dishes.end();
            return it->first;
        }
    }
    // If there is no id or the parameter is the wrong type(like 3), return a error situation
    return -1;
}
bool View_menu() {//This is the funcition for showing the menu
    if (dishes.empty()) {
        cout << "The chef or manager hasn't editted the menu." << endl;
        return false;
    }
    else {
        for (int i = 0; i <= GetMaxId_2(1); i++) {
            cout << i + 1 << ". " << dishes[i].Get_d_name() << "--------------" << dishes[i].Get_d_price() << "  yuan" << endl;
        }
        cout << GetMaxId_2(1) + 2 << ". " << "Exit ordering" << endl;
        return true;
    }
}
//Here i is the actual ID of the dishes, but this is a customer oriented function, which means that the indexes have to start at 1 instead of 0, so all the indexes have to be plus one, in which case the
//The menu index of the last dishes is GetMaxId(1)+1, so our rollout a la carte index is GetMaxId(1)+2

void d_edit(Dish& dish) {
    int type = GetMaxId_2(1);
    if (type == -1) {
        cout << "There no dish in the library, no metter whatever you enter, the id of the dish you are editting will be 0." << endl;
    }
    else {
        cout << "The maxmum id(m) of the existing dishes is " << type + 1 << ", and if you enter a number which is greater than id(m), the id of the dish you are editting will be " << type + 2 << endl;
        cout << "And if you enter a id which is smaller than " << type + 1 << ", you may change the existing dishes." << endl;
    }
    cout << "Please edit the id of the dish" << endl;
h5:int id;
    while (!(cin >> id)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (id < 0) {
        cout << "Please enter a number which is equal to 0 or larger than 0." << endl;
        goto h5;
    }
    dish.d_id = id + 1;
h7: cout << "Please edit the price of the dish" << endl;
    float price;
    while (!(cin >> price)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    dish.d_price = price;
    cout << "Please give the dish a name" << endl;
    string name;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (name.empty()) {
        cout << "Please edit the name of the dish in a correct way:" << endl;
        cin.clear();
        getline(cin, name);
    }
    dish.d_name = name;
    cout << "These are our total materials and their numbers:" << endl;
    bool MM = View_total_materials();
    cout << "Please decide how many kinds of the materials this dish needs:" << endl;
h0: int kind;
    while (!(cin >> kind)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (kind > GetMaxId_1(0) + 1) {
        cout << "The kind number of the materials is only " << GetMaxId_1(0) + 1 << ", please change the number of the kinds this dish you are editting." << endl;
        goto h0;
    }
    if (kind <= 0) {
        cout << "The kind should be greater than 0, please change." << endl;
        goto h0;
    }
    dish.material_kind = kind ;
    cout << "Now please edit the materials and the amount of the materials the dish needs, you need to provide the id of the material and the amount:" << endl;
    int id_ = -1;
    if (MM) {
        float cost = 0;
        vector<int> have_taken(dish.material_kind, -2);
        for (int i = 0; i < dish.material_kind; i++) {
            cout << "Please give the order number of the material kind:" << endl;
        h1: cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (!(cin >> id_)) {
                cout << "Invalid input, please enter a valid value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (id_ > GetMaxId_1(0) + 1) {
                cout << "Please pay attention to the list of the materials, the greates order of the materials is " << GetMaxId_1(0) + 1 << endl;
                cout << "Please change your order" << endl;
                goto h1;
            }
            bool have = true;
            for (int j = 0; j < dish.material_kind; j++) {
                if (have_taken[j] == id_) {
                    have = false;
                }
            }
            if (!have) {
                cout << "You have already applied this material, please change the number." << endl;
                goto h1;
            }
            have_taken[i] = id_;
            dish.material_table[0][i] = id_ - 1;
            cout << "Please edit how many this material the dish needs" << endl;
            int num;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (!(cin >> num)) {
                cout << "Invalid input, please enter a valid value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            dish.material_table[1][i] = num;
            cost = cost + materials[id_ - 1].Get_m_cost() * num;
        }
        dish.d_interest = dish.d_price - cost;
        if (dish.d_interest < 0) {
            cout << "The interest of this dish is less than 0, which means we will lose money if we sell this dish, would you like to edit the price of the dish and kinds and the number of the materials again?" << endl;
            cout << "1. Yes       2. No" << endl;
            int choice;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (!(cin >> choice)) {
                cout << "Invalid input, please enter a valid value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (choice == 1) {
                goto h7;
            }
        }
        cout << "You have operated successfully" << endl;
    }
    else {
        cout << "The names and the numbers of materials are needed to be editted." << endl;
    }
}

void d_delete(Dish& dish) {
    View_menu();
m0:
    cout << "Please input the id of the dish that you want to delete" << endl;
    int delete_dishid;
    cin >> delete_dishid;
    //judgemental input
    while (!delete_dishid) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (delete_dishid < 0) {
        cout << "Please enter a number which is equal to 0 or larger than 0." << endl;
        goto m0;
    }
    //delete
    if (dishes.find(delete_dishid) != dishes.end()) { // If the dish corresponding to this id is found
        dishes.erase(delete_dishid);
    }
    else {
        std::cout << "Dish with id " << delete_dishid << " not found." << std::endl;
    }
}


void View_Material_list(Dish& dish) {
    if (dish.material_table.empty()) {
        cout << "You haven't editted any materials for this dish." << endl;
    }
    else {
        bool allone = false;
        for (int i = 0; i < dish.material_kind; i++) {
            if (dish.material_table[0][i] != -1) {
                allone = true;
            }
        }
        if (!allone) {
            cout << "You haven't provide the name of the materials and the corresponding number." << endl;
        }
        else if (allone) {
            cout << "This is the raw material list:" << endl;
            for (int i = 0; i < dish.material_kind; i++) {
                if (dish.material_table[0][i] != -1 && dish.material_table[1][i] != -1) {
                    cout << "The name of the material and the amount: " << materials[dish.material_table[0][i]].Get_m_name() << "----" << dish.material_table[1][i] << endl;
                }
            }
        }
    }

}

//**********************************************************************Individual order

class Individual_order {
private:
    int I_id;
    int table_num;
    float I_price;
    float I_interest;
    vector<vector<int>>dish_table;
    int dish_kind;
    bool f = false;
    //Individual_order* I_next=NULL;
public:
    Individual_order(int id = 0, int num = 0, float price = 0.0, float interest = 0.0, int kind = GetMaxId_2(1)) :I_id(id), table_num(num), I_price(price), I_interest(interest), dish_kind(kind + 1) {
        if (dish_kind > 0) {
            dish_table.resize(2, vector<int>(dish_kind, -1));
        }
    }
    //Other functions
    int Get_I_id() { return I_id; }
    int Get_table_num() const { return table_num; }
    float Get_I_interest()const { return I_interest; }
    float Get_I_price() const { return I_price; }
    int Get_dish_kind() const { return dish_kind; }
    bool pay_or_not() const { return f; }
    void fix_I_id(int i) { I_id = i; }
    void fix_I_price(float newprice) { I_price = newprice; }
    void fix_I_interest(float newinterest) { I_interest = newinterest; }
    vector<vector<int>>& Get_dish_table() { return dish_table; }
    friend ostream& operator<<(ostream& os, const Individual_order& order) {
        os << order.I_id << " " << order.table_num << " "
            << order.I_price << " " << order.I_interest << " "
            << order.dish_kind << " " << order.f << " "; // Add Boolean f

        int numDishes = GetMaxId_2(1) + 1;
        if (numDishes > 0) {
            for (int i = 0; i < numDishes - 1; ++i) {
                os << order.dish_table[0][i] << " ";
            }
            os << order.dish_table[0][numDishes - 1] << "|"; // Separate first and second rows

            for (int i = 0; i < numDishes; ++i) {
                os << order.dish_table[1][i] << (i < numDishes - 1 ? " " : "");
            }
        }

        return os;
    }
    friend istream& operator>>(istream& is, Individual_order& order) {
        is >> order.I_id >> order.table_num >>
            order.I_price >> order.I_interest >> order.dish_kind >>
            order.f; //Read Boolean f

        int numDishes = GetMaxId_2(1) + 1;
        order.dish_table.resize(2, vector<int>(numDishes, -1));

        for (int i = 0; i < numDishes; ++i) {
            is >> order.dish_table[0][i];
        }
        is.ignore(); // Ignore vertical bars or whitespace characters after the first line

        for (int i = 0; i < numDishes; ++i) {
            is >> order.dish_table[1][i];
        }

        return is;
    }

    bool pay_bill() {
        cout << "Please make sure whether you'd like to pay the bill now or not:" << endl;
        cout << "1. Yes     2. Not now" << endl;
    h23:       int t;
        while (!(cin >> t)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (t != 1 && t != 2) {
            cout << "Please choose the operation number (1 or 2) and enter again." << endl;
            goto h23;
        }
        if (t == 1) {
            load_sum();
            sum = sum + I_interest;
            cout << "You have pay the bill successfully! Thanks for coming !" << endl;
            cout << "The sum of the money of the restaurant is " << sum << " now." << endl;
            update_sum();
            f = true;
        }
        else if (t == 2) {
            f = false;
        }
        return f;
    }


    friend void order(Individual_order& I);
    void View_this_order() {
        bool alone = false;
        for (int i = 0; i < dish_kind; i++) {
            if (dish_table[0][i] != -1 && dish_table[1][i] != -1) {
                alone = true;
            }
        }
        if (!alone) {
            cout << "Although this order exists, but customers haven't add something in it." << endl;
        }
        else {
            cout << "These are what this order has:" << endl;
            for (int j = 0; j < dish_kind; j++) {
                if (dish_table[0][j] != -1 && dish_table[1][j] != -1) {
                    cout << j + 1 << ". These are the name of the dish and the amount of the dishes and the cost:" << dishes[dish_table[0][j]].Get_d_name() << "----" << dish_table[1][j] << "----" << dishes[dish_table[0][j]].Get_d_price() * dish_table[1][j] << "yuan" << endl;
                }
            }
            cout << "Total cost: " << I_price << endl;
        }
        cout << "The meal number: " << I_id + 1 << endl;
        cout << "The table number: " << table_num << endl;
        if (!f) {
            cout << "The bill hasn't been paid" << endl;
        }
        else {
            cout << "The bill has been paid" << endl;
        }
    }
    void addition() {
        cout << "This is current information of this bill: " << endl;
        View_this_order();
        vector<vector<int>> t_table = dish_table;
        int i = 0;
        while (i < dish_kind) {
            if (t_table[0][i] == -1 && t_table[1][i] == -1) {
                break;
            }
            i++;
        }
        if (i == dish_kind) {
            cout << "Dear customer, you have ordered all the dishes, would you like to change the number of some dishes?" << endl;
            cout << "1. Yes        2. No" << endl;
            int choice;
            while (!(cin >> choice)) {
                cout << "Invalid input, please enter a valid value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (choice == 1) {
                int t;
                while (1) {
                    cout << "Please enter the index of the dish which you want to change" << endl;
                    cout << "If you want to exit, please enter " << i + 1 << endl;
                h9:                   while (!(cin >> t)) {
                    cout << "Invalid input, please enter a valid value." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (t <= 0 || t > i + 1) {
                    cout << "The order number does no exist, please change your number." << endl;
                    goto h9;
                }
                else if (t == i + 1) {
                    break;
                }
                else if (t > 0 && t < i + 1) {
                    cout << "Please give the number you want to change" << endl;
                    int t2;
                h10:                   while (!(cin >> t2)) {
                    cout << "Invalid input, please enter a valid value." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (t2 <= 0) {
                    cout << "The number should be greater than 0, please change the number." << endl;
                    goto h10;
                }
                dish_table[1][t - 1] = t2;
                }
                }
            }
        }
        else if (i < dish_kind - 1) {
            cout << "This is our menu, please feel free to add anything." << endl;
            bool V = View_menu();
            if (!V) {
                cout << "Sorry for that, we will edit the menu soon." << endl;
            }
            else if (V) {
                int index1;
                while (1) {
                    cout << "Please choose the dish order you want." << endl;
                h11:                  while (!(cin >> index1)) {
                    cout << "Invalid input, please enter a valid value." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (index1 <= 0 || index1 > GetMaxId_2(1) + 2) {
                    cout << "The order number is invalid, please change." << endl;
                    goto h11;
                }
                if (index1 == GetMaxId_2(1) + 2) {
                    break;
                }
                for (int j = 0; j < dish_kind; j++) {
                    int have = t_table[0][j];
                    if (have == index1 - 1) {
                        cout << "Dear customer, you have ordered this dish, please change the order number." << endl;
                        goto h11;
                    }
                }
                dish_table[0][i] = index1 - 1;
                t_table[0][i] = index1 - 1;
                cout << "Please edit the number of this dish" << endl;
                int num;
            h12:                  while (!(cin >> num)) {
                cout << "Invalid input, please enter a valid value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (num <= 0) {
                cout << "The number of the dishes should be greater than 0, please enter again." << endl;
                goto h12;
            }
            dish_table[1][i] = num;
            t_table[1][i] = num;
            i++;
                }
            }
        }
        float price = 0;
        int interest = 0;
        for (int in = 0; in < dish_kind; in++) {
            if (dish_table[0][in] != -1 && dish_table[1][in] != -1) {
                price = price + dishes[dish_table[0][in]].Get_d_price() * dish_table[1][in];
                interest = interest + dishes[dish_table[0][in]].Get_d_interest() * dish_table[1][in];
            }
        }
        cout << "Operated successfully!" << endl;
    }
    void decline() {
        View_this_order();
        bool alone = false;
        for (int i = 0; i < dish_kind; i++) {
            if (dish_table[0][i] != -1 && dish_table[1][i] != -1) {
                alone = true;
            }
        }
        if (!alone) {
            cout << "There's no information, please turn to ordering usage to order something." << endl;
        }
        else {
            cout << "Please choose the dish you want to delete" << endl;
            int t1;
        h13:                while (!(cin >> t1)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (t1 <= 0 || t1 > dish_kind) {
            cout << "The order you enter does not exit, please enter another number" << endl;
            goto h13;
        }if (dish_table[0][t1 - 1] != -1 && dish_table[1][t1 - 1] != -1) {
            bool alone3 = false;
            for (int j = t1 - 1; j < dish_kind; j++) {
                if (dish_table[0][j] != -1 && dish_table[1][j] != -1) {
                    alone3 = true;
                    for (int ii = 0; ii < dish_kind; ii++) {
                        if (dish_table[0][ii] == -1 && dish_table[1][ii] == -1) {
                            dish_table[0][ii] = dish_table[0][j];
                            dish_table[1][ii] = dish_table[1][j];
                            break;
                        }
                    }
                    dish_table[0][j] = -1;
                    dish_table[1][j] = -1;
                }
            }
            if (!alone3) {
                dish_table[0][t1 - 1] = -1;
                dish_table[1][t1 - 1] = -1;
            }
        }
        else {
            cout << "The order number also does not exist, please change your number" << endl;
            goto h13;
        }
        bool alone2 = false;
        for (int i = 0; i < dish_kind; i++) {
            if (dish_table[0][i] != -1 && dish_table[1][i] != -1) {
                alone2 = true;
            }
        }
        if (alone2) {
            int price = 0;
            int interest = 0;
            for (int in = 0; in < dish_kind; in++) {
                if (dish_table[0][in] != -1 && dish_table[1][in] != -1) {
                    price = price + dishes[dish_table[0][in]].Get_d_price() * dish_table[1][in];
                    interest = interest + dishes[dish_table[0][in]].Get_d_interest() * dish_table[1][in];
                }
            }
            I_price = price;
            I_interest = interest;
            cout << "Operated successfully!" << endl;
        }
        else {
            cout << "Now you do not order anything, please order something soon." << endl;
        }
        }
    }

    ~Individual_order() {}
};
map<int, Individual_order> orders;
int GetMaxId_3() {
    if (!orders.empty()) {
        auto it = --orders.end();
        return it->first;
    }
    return -1;
}

void order(Individual_order& I) {
    cout << "This is our menu, please take a look at it:" << endl;
    bool V = View_menu();
    if (V) {
        cout << "Please choose your table number ( our restaurant only have 10 tables, please choose the number from 1-10):" << endl;
    h4:         int t_num;
        while (!(cin >> t_num)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (t_num < 1 || t_num>10) {
            cout << "Please choose the table number correctly (1-10)." << endl;
            goto h4;
        }
        I.table_num = t_num;
        int t1 = -1;
        int t2 = 0;
        vector<int> have_ordered(I.dish_kind, -2);
        float price = 0;
        float interest = 0;
        cout << "Thanks for ordering, please feel free to order anything." << endl;
        while (1) {
            cout << "Please enter the index of the dishes" << endl;
        h6:             while (!(cin >> t1)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (t1 == GetMaxId_2(1) + 2) {
            break;
        }
        if (t1 > GetMaxId_2(1) + 2 || t1 <= 0) {
            cout << "We do not have this order number, please enter again:" << endl;
            goto h6;
        }
        bool have = true;
        for (int i = 0; i < I.dish_kind; i++) {
            if (t1 == have_ordered[i]) {
                have = false;
            }
        }
        if (!have) {
            cout << "Dear customer, you have already chosen this dish, please change to another one." << endl;
            goto h6;
        }
        have_ordered[t2] = t1;
        I.dish_table[0][t2] = t1 - 1;
        int t3;
        cout << "How many do you want:" << endl;
    h8:       while (!(cin >> t3)) {
        cout << "Invalid input, please enter a valid value." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (t3 <= 0) {
        cout << "The number should be greater than 0, please enter again." << endl;
        goto h8;
    }
    I.dish_table[1][t2] = t3;
    price = price + dishes[t1 - 1].Get_d_price() * t3;
    interest = interest + dishes[t1 - 1].Get_d_interest() * t3;
    t2++;
        }
        I.I_price = price;
        I.I_interest = interest;
        cout << "Thanks for ordering!" << endl;
    }
    else if (!V) {
        cout << "Sorry for that, we will edit the menu soon." << endl;
    }
    I.I_id = GetMaxId_3() + 1;
    cout << "The meal number of your dishes is: " << I.I_id + 1 << endl;
}

void assignMaterial(map<int, Material>& materials, Material M) {
    if (materials.empty()) {
        M.fix_m_id(0);
        materials[0] = M;
    }
    else {
        int id = M.Get_m_id();
        auto it = materials.find(id);
        if (it == materials.end()) {
            auto _it = --materials.end();
            if (id > _it->first) {
                id = _it->first + 1;
                M.fix_m_id(id);
                materials[id] = M;
            }
        }
        else {
            materials.erase(it);
            materials[id] = M;
        }
    }
}
//invoke a method：assignMaterial(materials, M);
void assignDish(map<int, Dish>& dishes, Dish D) {
    if (dishes.empty()) {
        D.fix_id(0);
        dishes[0] = D;
    }
    else {

        int id = D.Get_d_id();
        auto it = dishes.find(id);
        if (it == dishes.end()) {
            auto _it = --dishes.end();
            if (id > _it->first) {
                id = _it->first + 1;
                D.fix_id(_it->first + 1);
                dishes[id] = D;
            }
        }
        else {
            dishes.erase(it);
            dishes[id] = D;
        }
    }
}
//invoke a method：assignDish(dishes,D)

void assignOrder(map<int, Individual_order>& orders, Individual_order O) {
    if (orders.empty()) {
        O.fix_I_id(0);
        orders[0] = O;
    }
    else {
        int id = O.Get_I_id();
        auto it = orders.find(id);
        if (it == orders.end()) {
            auto _it = --orders.end();
            if (id > _it->first) {
                id = _it->first + 1;
                O.fix_I_id(_it->first + 1);
                orders[id] = O;
            }
        }
        else {
            orders.erase(it);
            orders[id] = O;
        }
    }
}





//**********************************************************************File Read| CustomerService class

template<typename T>
void fileWrite(const std::map<int, T>& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::out); // Opening a file in text mode
    if (file.is_open()) {
        for (const auto& pair : data) {
            // Assuming the T type has an overloaded << operator.
            file << pair.first << " " << pair.second << std::endl;
        }
    }
    else {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    }
}

// Functions to deserialise a map from a text file
template <typename T>
void fileRead(std::map<int, T>& data, const std::string& filename) {
    std::ifstream file(filename, std::ios::in); // Opening a file in text mode
    if (file.is_open()) {
        int key;
        T value; // Assuming that type T has overloaded >> operators
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> key >> value) {
                data[key] = value;
            }
        }
    }
    else {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
    }
}
void write() {
    if (!materials.empty()) {
        fileWrite(materials, "Materials.txt");
    }
    if (!dishes.empty()) {
        fileWrite(dishes, "Dishes.txt");
    }
    if (!orders.empty()) {
        fileWrite(orders, "Orders.txt");
    }
}
bool IsFileEmpty(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;

    if (!file) {
        std::cerr << filePath << " file is empty." << std::endl;
        return true; // The file cannot be opened, return true means empty or cannot be judged.
    }

    // Try to read at least one line
    while (getline(file, line)) {
        // If a non-blank line is found, the file is not empty
        if (!line.empty() && line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
            return false;
        }
    }
    return true; // The file may contain only blank or empty
}
void load() {
    if (!IsFileEmpty("Materials.txt")) {
        fileRead(materials, "Materials.txt");
    }
    if (!IsFileEmpty("Dishes.txt")) {
        fileRead(dishes, "Dishes.txt");
    }
    if (!IsFileEmpty("Orders.txt")) {
        fileRead(orders, "Orders.txt");
    }
}

//********************************************************************** CustomerService


using namespace std;

using namespace std;

class CustomerService {
public:
    void view_menu() {
        bool success = View_menu();
        if (!success) {
            std::cout << "No dishes available in the menu." << std::endl;
        }
    }

    virtual void place_order() {
        Individual_order newOrder;
        order(newOrder);
        assignOrder(orders, newOrder);
    }

    void Addition() {
        cout << "These are all meal numbers we have: " << endl;
        for (int i = 0; i <= GetMaxId_3(); i++) {
            cout << i + 1 << "  ";
        }
        cout << endl << "Please give your meal number." << endl;
    h20:        int num;
        while (!(cin >> num)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (num <= 0 || num > GetMaxId_3() + 1) {
            cout << "We do not have the meal numer like this, please make sure again." << endl;
            goto h20;
        }
        orders[num - 1].addition();
    }

    void Decline() {
        cout << "These are all meal numbers we have: " << endl;
        for (int i = 0; i <= GetMaxId_3(); i++) {
            cout << i + 1 << " ";
        }
        cout << endl << "Please give your meal number." << endl;
    h21:        int num;
        while (!(cin >> num)) {
            cout << "Invalid input, please enter a valid value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (num <= 0 || num > GetMaxId_3() + 1) {
            cout << "We do not have the meal numer like this, please make sure again." << endl;
            goto h21;
        }
        orders[num - 1].decline();
    }

    void pay_bill() {
        cout << "These are all meal numbers we have: " << endl;
        for (int i = 0; i <= GetMaxId_3(); i++) {
            cout << i + 1 << " ";
        }
        std::cout << endl << "Enter the order ID you want to pay for: ";
        int orderId = get_validated_input();
        auto it = orders.find(orderId - 1);
        if (it != orders.end()) {
            if (it->second.pay_or_not()) {
                cout << "This order has been paid by customers." << endl;
            }
            else {
                if (!it->second.pay_bill()) {
                    std::cout << "Payment unsuccessful, this order is still waiting to be paid for." << std::endl;
                }
            }
        }
        else {
            std::cout << "Order ID not found." << std::endl;
        }
    }
    int get_validated_input() {
        int value;
        while (!(std::cin >> value)) {
            std::cout << "Invalid input, please enter a valid value." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return value;
    }
    virtual ~CustomerService() {};
};



//**********************************************************************  User class: mainly used for VIP account related
class User :public CustomerService {
private:

protected:
    string u_name, u_password;
    int u_id;
    static int userCount;
public:
    int Get_u_id() { return u_id; }
    string Get_u_name() { return u_name; }
    string Get_u_password() { return u_password; }
    static int Get_u_userCount;
    static int get_u_NextID() {
        return userCount++;
    }
    User(int id = 1, string name = "", string password = "") :u_id(id), u_name(name), u_password(password)
    {
        if (id == 0) { /*if try to create id = 0，wrong because 0 is reserved*/
            throw std::invalid_argument("ID 0 is reserved for Manager.");
        }
    }
    /* get Next customer'sID */
    static int getNextID() {
        return userCount++;
    }
    void u_edit(string name, string password) {
        u_name = name;
        u_password = password;
    }
    void edit_u_id(int id) { u_id = id; }
    void place_order() {
        Individual_order newOrder;
        order(newOrder);
        float originalPrice = newOrder.Get_I_price();
        newOrder.fix_I_price(originalPrice = originalPrice * 0.8); // 20% off for VIP users
        assignOrder(orders, newOrder);
    }
    virtual~User() {}
    /*for fileRead and write*/
     //operator overloading，for output User class to ostream
    friend std::ostream& operator<<(std::ostream& os, const User& mat) {
        os << mat.u_id << " " << mat.u_name << " " << mat.u_password << " " << mat.userCount;
        return os;
    }
    /* read from istream for User class*/
    friend std::istream& operator>>(std::istream& is, User& mat) {
        is >> mat.u_id >> mat.u_name >> mat.u_password >> mat.userCount;
        return is;
    }
};
int User::userCount = 1; //user id starts from 1 and then increases, 0 is reserved for manager

map<int, User> users;
int GetMaxId_user() { /*The function which can get the maximum number of the id of the existent classes*/
    if (!users.empty()) {
        auto it = --users.end();
        return it->first;
    }
    /*If there is no id or the parameter is the wrong type(like 3), return a error situation*/
    return -1;
}

//VIP accounts register


int addUser(User& U) {
    int id = 1 + User::get_u_NextID();  // Get the maximum user ID and add one
    users[id] = U;
    U.edit_u_id(id);
    fileWrite(users, "User.txt");
    return id;
}

int editUser(int id) {
    auto it = users.find(id);
    if (it != users.end()) {
        cout << "Please enter the new name of the user that you want to edit:";
        string newName;
        cin >> newName;
        cout << "Please enter the new password of the user that you want to edit:";
        string newPassword;
        cin >> newPassword;
        it->second.u_edit(newName, newPassword);  // Change the user name in the place of the original ID.
        fileWrite(users, "User.txt");
        std::cout << "User edited successfully\n";
        return 0;
    }
    else {
        std::cout << "User with ID " << id << " does not exist." << std::endl;
        return -1;
    }
}

void deleteUser(int id) {
    auto it = users.find(id);
    if (it != users.end()) {
        users.erase(it);
        fileWrite(users, "User.txt");
        std::cout << "User with ID " << id << " has been deleted." << std::endl;
    }
    else {
        std::cout << "User with ID " << id << " does not exist." << std::endl;
    }
}

bool View_total_VIPuser() {
    if (users.empty()) {
        cout << "There are no vip users yet" << endl;
        return false;
    }
    else {
        auto it = users.lower_bound(1);  // Find the first location where the key value is greater than or equal to 1
        for (; it != users.end(); it++) {
            cout << it->first << ". " << it->second.Get_u_name() << endl;
        }
        cout << "\n";
        return true;
    }
}


//VIP accounts login
int user_login()  //return -2: invalid; return -1: wrong entry; return other: user id
{
    fileRead(users, "User.txt");
    //accounts_load();
    int i, op;
    User u;
    int usernum = 1 + GetMaxId_user();
    std::cout << "\n*1. Log in        *";
    std::cout << "\n*2. Register      *\n" << endl;
    std::cout << " Please input a number of your choice: ";
    cin >> op;
    if (op == 1) //log in
    {
        std::string name, password;
        std::cout << "\n**********Log in**********\nUsername:";
        cin >> name;
        std::cout << "Password:";
        cin >> password;
        int err = 1;
        for (i = 0; i < usernum; i++)
        {
            if (name == users[i].Get_u_name())
            {
                err = 0;
                break;
            }
        }
        if (err)
        {
            std::cout << "\nThis username does not exist!\n";
            return -1; //return -1 for not existing username
        }
        else
        {
            if (password != users[i].Get_u_password())
            {
                std::cout << "\nIncorrect password!\n";
                return -1; //return -1 for wrong password 
            }
            else
            {
                system("CLS");
                std::cout << "\nVIP Log in Success\n";
                return i; //return users id
            }
        }
    }
    else if (op == 2) {
        int err = 0;
        string name, passsword;
        std::cout << "\n**********Register**********\n";
        std::cout << "Username:";
        std::cin >> name;
        for (i = 0; i < usernum; i++)
        {
            if (name == users[i].Get_u_name())
            {
                err = 1;  /* test errors of used username*/
                break;
            }
        }
        if (err)
        {
            std::cout << "\nThis username exists!\n";
            return -1; //return -1 for existing username
        }
        std::cout << "password:";
        std::cin >> passsword;
        u.u_edit(name, passsword);
        addUser(u);
        cout << "membership added successfully\n";
    }
    else return -2;
}

//**********************************************************************Chef class
class Chef {
    string c_name, c_password;
    int c_id;
    static int chefCount;
public:
    Dish chef_dish;
    Chef() :c_id(0), c_name(""), c_password("123") {}
    void c_edit(int id, string name, string password = "123") {
        c_id = id;
        c_name = name;
    }
    void c_edit(int id) { c_id = id; }
    int Get_c_id() { return c_id; }
    string Get_c_name() { return c_name; }
    string Get_c_password() { return c_password; }
    static int Get_c_chefCount;
    static int getNextID() {
        return chefCount++;
    }
    /*Menu function*/
    // Function 1: Browse all existing materials
    bool chef_view_all_material() {
        load();
        return View_total_materials();
    }

    // Function 2: Search if there is a certain raw material
    bool chef_search_material(string searching_name_material) {
        load();
        cout << "What is the material you want to search for?" << endl;
        cin >> searching_name_material;
        for (const auto& pair : materials) {
            string name = pair.second.Get_m_name();
            if (name == searching_name_material) {
                cout << "Already have material called " << searching_name_material << endl;
                return true;
            }
        }
        cout << "No material was found: " << searching_name_material << endl;
        cout << "You can look for the manager and ask him to add the material." << endl;
        return false;
    }

    // (New) Function 3: View the existing menu -> See what dishes are currently available
    bool chef_viewDish() {
        load();
        return View_menu();
    }

    // (New) Function 4: Search for a dish -> Available (can view composition directly) / Not available (return not found)
    bool chef_searchDish(string searching_name_dish) {
        load();
        Dish dish;
        cout << "What is the name of the dish you want to search for?" << endl;
        cin >> searching_name_dish;
        for (int i = 0; i <= GetMaxId_2(1); i++) {
            if (searching_name_dish == dishes[i].Get_d_name()) {
                cout << "Already have this dish, and the recipe for the dish is as follows" << endl;

                View_Material_list(dishes[i]);
                return true;
            }
        }
        cout << "No dish was found: " << searching_name_dish << endl;
        cout << "You can create one." << endl;
        return false;
    }

    // Function 5: Edit the menu -> Add new dishes
    void chef_d_edit(Dish& mydish) {
        load();
        d_edit(mydish);
        assignDish(dishes, mydish);
        write();
    }

    //Function 6: View the menu for table
    void chef_view_table_menu() {
        load();
        int total_order_id = GetMaxId_3() + 1;
        cout << "There are " << total_order_id << " order" << endl;
        cout << "Please enter the id of order that you want to check" << endl;
        int searching_id;
        cin >> searching_id;
        Individual_order O = orders[searching_id - 1];
        O.View_this_order();
    }

    /*for fileRead and write*/
    friend std::ostream& operator<<(std::ostream& os, const Chef& mat) {
        os << mat.c_id << " " << mat.c_name << " " << mat.c_password << " " << mat.chefCount;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Chef& mat) {
        is >> mat.c_id >> mat.c_name >> mat.c_password >> mat.chefCount;
        return is;
    }

    friend class Manager;
    ~Chef() {}

};
int Chef::chefCount = 0;//chef id starts from 0 and then increases

map<int, Chef> chefs;

int GetMaxId_chef() { /*The function which can get the maximum number of the id of the existent classes*/
    if (!chefs.empty()) {
        auto it = --chefs.end();
        return it->first;
    }
    /*If there is no id or the parameter is the wrong type(like 3), return a error situation*/
    return -1;
}

void assignChef(map<int, Chef>& chefs, Chef C) {
    int id = C.Get_c_id();
    auto it = chefs.find(id);
    if (it == chefs.end()) { //id Unoccupied
        auto _it = --chefs.end();
        if (id > _it->first) {//Chef id > last one's id in map
            id = _it->first + 1; //put into the next
            chefs[id] = C;
        }
    }
    else {//id Unoccupied
        chefs.erase(it);
        chefs[id] = C;
    }

}

bool view_all_chef() {
    if (chefs.empty()) {
        cout << "There are no chefs yet" << endl;
        return false;
    }
    else {
        cout << "Here are the current chefs of our restaurant:\n";
        auto it = chefs.lower_bound(0);  // Find the first key value greater than or equal to 0
        for (; it != chefs.end(); it++) {
            cout << (it->first) + 1 << ". " << it->second.Get_c_name() << endl;
        }
        cout << "\n";
        return true;
    }
}

//Chef accounts editing
void addChef() {
    cout << "\nplease type number of chefs you want to add:\n";
    int num = 1;
    cin >> num;
    cout << "\n***NOTICE:Chef account's password is initialized to 123***\n";
    for (int i = 0; i < num; i++) {
        cout << "NO." << i + 1 << " chef account user name:";
        string chefname;
        cin >> chefname;
        Chef* c = new Chef();
        int id = Chef::getNextID();
        c->c_edit(id, chefname);
        chefs[id] = *c;
        fileWrite(chefs, "Chef.txt");  /*initialize chef and save to files*/
        delete(c);
    }
    if (num > 0) cout << "\nchef account registration successful!\n";
}

int editChef(Chef& newChef) {
    auto it = chefs.find(newChef.Get_c_id());
    if (it != chefs.end()) { // Chef account exists
        it->second = newChef; // Replacement of old information with new chef information
        fileWrite(chefs, "Chef.txt");  // Write the changed chef information to a file
        std::cout << "Chef account edited successfully\n";
        return 0;
    }
    else { // Chef account does not exist
        std::cout << "Chef account with ID " << newChef.Get_c_id() << " does not exist." << std::endl;
        return -1;
    }
}

void chef_login() throw(int) {
    system("CLS");
    int i, op;
    int chefnum = 1 + GetMaxId_chef();
    fileRead(chefs, "Chef.txt");
    while (1) {
        std::cout << "1:Comfirm to log in as chef.   2:Exit\n";
        cin >> op;
        if (op == 1) //log in
        {
            system("CLS");
            std::string name, password;
            std::cout << "\n**********Chef Log in**********\nUsername:";
            cin >> name;
            std::cout << "Password:";
            cin >> password;
            int err = 1;
            for (i = 0; i < chefnum; i++)
            {
                if (name == chefs[i].Get_c_name())
                {
                    err = 0;
                    break;
                }
            }
            if (err)
            {
                std::cout << "\nThis username does not exist!\n\n";
                //loop agian for not existing username
            }
            else
            {
                if (password != chefs[i].Get_c_password())
                {
                    std::cout << "\nIncorrect password!!\n\n";
                    //loop again for wrong password 
                }
                else
                {
                    system("CLS");
                    std::cout << "\nChef Log in Success\n\n";
                    break;
                    //jump out the loop for success login
                }
            }
        }
        else throw(-1);
    }
}

//**********************************************************************Manager class: user for manager menu
class Manager :public User {
public:
    Manager(string name = "", string password = "") {
        u_id = 0;  // manager id is fixed to cons number 0
        u_name = name;
        u_password = password;
    }

    /*menu Function*/

     //Function 1: View vip users
    bool manager_u_view() {
        return View_total_VIPuser();
    }

    //Function 2: Add vip users
    void manager_u_add(User& u) {
        string name;
        string password;
        cout << "Please enter the name of new vip: ";
        cin >> name;
        cout << "Please enter the password of new vip: ";
        cin >> password;
        u.u_edit(name, password);
        int newUserID = addUser(u);  // Call the add user function and get the ID of the new user
        cout << "The operation of adding VIP user is successful! Your user ID is: " << newUserID << endl;
    }

    //Function 2: Edit vip users
    void manager_u_edit() {
        User u;
        cout << "Here are the current VIP customers of our restaurant:\n";
        View_total_VIPuser();
        cout << "Please enter the id of the user that you want to edit:";
        int edit_userid;
        cin >> edit_userid;
        editUser(edit_userid);
        cout << "The operation of editing VIP user is successful!";
    }

    //Function 3: Delete vip users
    void manager_u_delete() {
        View_total_VIPuser();
        cout << "Please enter the id of the user that you want to delete:" << endl;
        int delete_userid;
        cin >> delete_userid;
        deleteUser(delete_userid);
        cout << "The operation of deleting VIP user is successful!";
    }

    //Function4：view chef
    bool manager_c_view() {
        return view_all_chef();
    }

    //Function5: add chef
    void manager_c_add() {
        addChef();
    }

    //Function6: edit chef
    int manager_c_edit() {
        view_all_chef();
        cout << "Please input the ID of the chef you want to edit: ";
        int chef_id;
        cin >> chef_id;

        // Getting and modifying new chef information
        Chef newChef;
        newChef.c_edit(chef_id);

        int result = editChef(newChef);   // Calling the editChef function to modify existing chef information

        if (result == 0) {
            cout << "Chef account edited successfully!" << endl;
        }
        else {
            cout << "Failed to edit chef account" << endl;
        }

        return result;
    }


    //Function7: delete chef
    int manager_c_delete() {

        view_all_chef();

        cout << "Please input the ID of the chef you want to delete: ";
        int chef_id;
        cin >> chef_id;

        auto it = chefs.find(chef_id);
        if (it == chefs.end()) {
            std::cout << "Chef account not found" << endl;
            return -1;
        }
        else {
            chefs.erase(it);
            cout << "Chef account deleted successfully!" << endl;
            fileWrite(chefs, "Chef.txt"); // Save the updated chef data to file
            return 0;
        }
    }

    //Function8：View material
    bool manager_m_view() {
        load();
        return View_total_materials();
    }
    //Function9：Add material
    void manager_m_add(Material& new_material) {
        load();
        m_add(new_material);
        assignMaterial(materials, new_material);
        write();
    }

    //Function10：Edit material
    void manager_m_edit(Material& my_materials) {
        load();
        m_edit(my_materials);
        assignMaterial(materials, my_materials);
        write();
    }


    //Function11：delete material
    void manager_m_delete(Material& my_materials) {
        load();
        m_delete(my_materials);
        assignMaterial(materials, my_materials);
        write();
    }

    // Function12：view dish
    bool manager_d_view() {
        load();
        return View_menu();
    }

    // Function13：edit dish
    void manager_d_edit(Dish& my_dish) {
        load();
        d_edit(my_dish);
        assignDish(dishes, my_dish);
        write();
    }

    // Function14：delete dish
    void manager_d_delete(Dish& mydish) {
        load();
        d_delete(mydish);
        assignDish(dishes, mydish);
        write();

    }

    //Function 15: View Order Information
    void manager_view_table_menu() {
        load();
        int total_order_id = GetMaxId_3() + 1;
        cout << "There are " << total_order_id << " order" << endl;
        cout << "Please enter the id of order that you want to check" << endl;
        int searching_id;
        cin >> searching_id;
        Individual_order O = orders[searching_id - 1];
        O.View_this_order();
    }


    //Function 16: Check if the table number has been paid
    bool m_pay_bill() {
        Individual_order manager_check_order;
        load();
        int searching_order_id;
        int total_order_num = GetMaxId_3() + 1;
        cout << "There are now" << total_order_num << " orders in total.\n" << "Enter the order number you want to see if it is paid:";
        cin >> searching_order_id;

        bool pay_situation;
        pay_situation = manager_check_order.pay_or_not();

        if (pay_situation) {
            cout << "This order has been paid." << endl;
        }
        else {
            cout << "This order has not been paid." << endl;
        }
        return pay_situation;

    }

    //Function 17: View all money earned now
    void manager_check_sum() {
        float sum = load_sum();
        cout << "The total profit now is " << sum << endl;
    }
    ~Manager() {};
};

//manager login: compare with the class User in users[0]
void manager_login() throw(int) //return -1: fail;
{
    system("CLS");
    while (1) {
        int op, err = 0;
        std::cout << "1:Comfirm to log in as manager.   2:Exit\n";
        cin >> op;
        if (op == 1) //log in
        {
            system("CLS");
            std::string name, password;
            std::cout << "\n**********Manager Log in**********\nUsername:";
            cin >> name;
            std::cout << "Password:";
            cin >> password;
            if (name != users[0].Get_u_name()) {
                err = 1;
                std::cout << "\nINVALID name.You can't log in as manager!\n\n\n";
            } //loop again for wrong manager username
            else
            {
                if (password != users[0].Get_u_password())
                {
                    std::cout << "\nIncorrect password!\n";
                    //loop again for wrong password 
                }
                else
                {
                    system("CLS");
                    std::cout << "\n% Manager Log in Success\n\n";
                    break;
                    //break the loop for login success
                }
            }
        }
        else throw(-1);
    }
}


//**********************************************************************load and initialze functions
int beginning()
{
    std::cout << ("No file detected. Do you want to run the initialization setup program?\n1. Yes, run\t2. No, exit\n");
    int op;
    cin >> op;
    if (op == 1)
    {
        std::cout << "\nWelcome to the Restaurant Management System for the first time!\nNow beginning the initialization setup.\nplease type Manager username:";
        std::string username, password;
        cin >> username;
        std::cout << "Please define manager password :";
        cin >> password;
        try {
            Manager M(username, password); /*define a Manager，means its base class User's id is initialized by 0*/
            users[0] = M;
            fileWrite(users, "User.txt");    /*initalize Manager and save to files*/
            std::cout << "\nmanager account registration successful!\nNow start chef accounts activation for restaurant management system" << endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error initializing manager: " << e.what() << std::endl;
            /*exception handle*/
        }
        addChef();
        std::cout << "System setup completed!\nNow, please log in with the manager account\n\n";
        return 0;
    }
    else
        return -1;
}

int accounts_load() {
    fileRead(users, "User.txt");
    fileRead(chefs, "Chef.txt");
    if (users.empty() || chefs.empty()) {
        if (beginning() == 0) {
            return 0; /*initialized success:*/
        }
        else exit(-1); /*no initialize*/
    }
    else cout << "% data loading success\n\n";
}
//**********************************************************************System menu
class RestaurantManagementSystem {
public:
    void mainMenu() {
        accounts_load();
        while (true) {
        c0:
            std::cout << "\n *********** Main Menu ***********" << endl;
            std::cout << " * 1: customer service           *" << endl;
            std::cout << " * 2: chef                       *" << endl;
            std::cout << " * 3: manager                    *" << endl;
            std::cout << " * 4: quit the program           *" << endl;
            std::cout << " *********************************" << endl;
            std::cout << " Please input a number of your choice: ";
            int numb;
            std::cin >> numb;
            system("CLS");
            switch (numb) {
                /*customer service*/
            case 1: {
                int choice, user_id;
                std::cout << " * 1: take a normal order     *" << endl;
                std::cout << " * 2: menbership related      *" << endl << endl;
                std::cout << " Please input a number of your choice: ";
                std::cin >> choice;
                system("CLS");
                /* normal order*/
                if (choice == 1) { user_id = -1;  /*normal customer has Idof -1*/ }
                /* membership login*/
                else if (choice == 2) {
                    cout << "Dear customer, if you can log in the VIP account, you can enjoy a 20% discount!" << endl;
                    user_id = user_login();/*only membership has positive Id*/
                    if (user_id == -2) {
                        cout << "Fail to log in as a membership\n";
                        exit(-1);
                    }
                    else if (user_id == -1) goto c0;
                }
                else {
                    std::cout << "Invalid!!Fail to log in as a membership\n";
                    exit(-1);
                }
                customerMenu(user_id);
                break; }
                  /*chef */
            case 2: {
                try {
                    accounts_load();
                    chef_login();
                    chefMenu();
                }
                catch (int) {
                    std::cout << "Invalid!!Fail to log in as a chef\n";
                    exit(-1);
                }
                break;
            }
                  /* manager*/
            case 3: {
                try {
                    manager_login();
                    managerMenu();
                }
                catch (int) {
                    std::cout << "Invalid!!Fail to log in as a manager\n";
                    exit(-1);
                }
                break;
            }
            case 4:
                exit(0);
                break;
            default:
                std::cout << "invaild input" << std::endl;
                break;

            }

        }
    }
    void chefMenu() {
        load();
        Chef c;
        Dish chef_dish;
        while (true) {
        m0:
            std::cout << "\n *************** Chef Menu ****************" << endl;
            std::cout << " * 1: view material menu                  *" << endl;
            std::cout << " * 2: serach material                     *" << endl;
            std::cout << " * 3: view dish menu                      *" << endl;
            std::cout << " * 4: search dish                         *" << endl;
            std::cout << " * 5: edit dish                           *" << endl;
            std::cout << " * 6: View the menu for table             *" << endl;
            std::cout << " * 7: edit account                        *" << endl;
            std::cout << " * 8: quit and return main menu           *" << endl;;
            std::cout << " ******************************************\n" << endl;;
            std::cout << " Please input a number of your choice: ";
            int num_c;
            std::cin >> num_c;
            string material_name;
            string dish_name;
            switch (num_c) {
            case 1:
                load();
                system("CLS");
                cout << "% View material menu" << endl;
                c.chef_view_all_material();
                goto m0;
                break;
            case 2:
                load();
                system("CLS");
                cout << "% search material" << endl;
                c.chef_search_material(material_name);
                goto m0;
                break;
            case 3:
                load();
                system("CLS");
                cout << "% View dish menu" << endl;
                c.chef_viewDish();
                goto m0;
                break;
            case 4:
                load();
                system("CLS");
                cout << "% Search dish" << endl;
                c.chef_searchDish(dish_name);
                goto m0;
                break;
            case 5:
                load();
                system("CLS");
                cout << "% Edit dish" << endl;
                c.chef_d_edit(chef_dish);
                write();
                goto m0;
                break;
            case 6:
                load();
                system("CLS");
                cout << "% View the menu for table" << endl;
                c.chef_view_table_menu();
                goto m0;
                break;
            case 7:
                load();
                system("CLS");
                cout << "% Edit account" << endl;
                chef_login();
                break;
            case 8:
                load();
                system("CLS");
                cout << "% move to main menu and log out!" << endl;
                mainMenu();
                return;
            default:
                cout << "invaild input" << endl;
                break;

            }
        }
    }

    void managerMenu() {
        while (true) {
            //load();
            Manager m;
            User u;
            Material m_material;
            Dish m_dish;
        m1:
            cout << "\n ************** Manager Menu **************" << endl;
            cout << " * 1: Manage VIP users                    *" << endl;
            cout << " * 2: Manage chef                         *" << endl;
            cout << " * 3: Manage material                     *" << endl;
            cout << " * 4: Manage dish                         *" << endl;
            cout << " * 5: View the menu for table             *" << endl;
            cout << " * 6: Check the payment with table        *" << endl;
            cout << " * 7: total profits                       *" << endl;
            cout << " * 8: quit and return main menu           *" << endl;
            cout << " ******************************************" << endl;;
            cout << " Please input a number to run the program: ";
            int num_m;
            std::cin >> num_m;
            switch (num_m) {
            case 1://edit vip user
            m1_1:
                system("CLS");
                cout << "% Manage VIP user\n" << endl;
                int m_choice1;
                cout << "\n *********** Manager Menu - VIP ***********" << endl;
                cout << " * 1: view VIP user                       * " << endl;
                cout << " * 2: add new VIP user                    * " << endl;
                cout << " * 3: edit VIP user                       * " << endl;
                cout << " * 4. delete VIP user                     * " << endl;
                cout << " * 5. move to the last page               * " << endl;
                cout << " * 6: move to the main menu page          * " << endl;
                cout << " ******************************************" << endl;;
                cout << " Please input a number of your choice: ";
                cin >> m_choice1;
                switch (m_choice1) {
                case 1:
                    system("CLS");
                    cout << "% View VIP user" << endl;
                    m.manager_u_view();
                    break;
                case 2:
                    system("CLS");
                    cout << "% Add new VIP user" << endl;
                    m.manager_u_add(u);
                    goto m1_1;
                    break;
                case 3:
                    system("CLS");
                    cout << "% Edit VIP user" << endl;
                    m.manager_u_edit();
                    goto m1_1;
                    break;
                case 4:
                    system("CLS");
                    cout << "% Delete VIP user" << endl;
                    m.manager_u_delete();
                    goto m1_1;
                    break;
                case 5:
                    system("CLS");
                    cout << "% Move to the previous page!" << endl;
                    goto m1;
                case 6:
                    system("CLS");
                    cout << "% Remove the main menu and log out!" << endl;
                    return;

                default:
                    cout << "invaild input" << endl;
                    goto m1_1;
                    break;
                }
                goto m1;
                break;

            case 2://edit chef
                cout << "query product function" << endl;
            m1_2:
                system("CLS");
                cout << "% Manage chef\n" << endl;
                int m_choice2;
                cout << "\n ********* Manager Menu - Chef *********" << endl;
                cout << " * 1: view chef list                   * " << endl;
                cout << " * 2: add chef                         * " << endl;
                cout << " * 3: edit chef                        * " << endl;
                cout << " * 4: delete chef                      * " << endl;
                cout << " * 5: move to the previous page        * " << endl;
                cout << " * 6: move to the main menu page       * " << endl;
                cout << " ***************************************" << endl;
                cout << "Please input a number of your choice:" << endl;
                cin >> m_choice2;
                switch (m_choice2) {
                case 1:
                    system("CLS");
                    cout << "% View chef list" << endl;
                    m.manager_c_view();
                    break;
                case 2:
                    system("CLS");
                    cout << "% Add chef" << endl;
                    m.manager_c_add();
                    goto m1_2;
                    break;
                case 3:
                    system("CLS");
                    cout << "% Edit chef" << endl;
                    m.manager_c_edit();
                    goto m1_2;
                    break;
                case 4:
                    system("CLS");
                    cout << "% Delete chef" << endl;
                    m.manager_c_delete();
                    goto m1_2;
                    break;
                case 5:
                    system("CLS");
                    cout << "% Move to the previous page!" << endl;
                    goto m1;
                    break;
                case 6:
                    system("CLS");
                    cout << "% Remove the main menu and log out!" << endl;
                    return;
                default:
                    system("CLS");
                    cout << "invaild input" << endl;
                    goto m1_2;
                    break;
                }
                goto m1;
                break;
            case 3://edit material
            m1_3:
                system("CLS");
                cout << "% Manage material\n" << endl;
                int m_choice3;
                cout << "\n ********* Manager Menu - Material *********" << endl;
                cout << " * 1: view material                        * " << endl;
                cout << " * 2: add new material                     * " << endl;
                cout << " * 3: edit material                        * " << endl;
                cout << " * 4. delete material                      * " << endl;
                cout << " * 5. return to the previous screen        * " << endl;
                cout << " * 6: quit and return main menu            *" << endl;
                cout << " *******************************************" << endl;;
                cout << " Please input a number of your choice: ";

                cin >> m_choice3;
                switch (m_choice3) {
                case 1:
                    system("CLS");
                    cout << "% View Material" << endl;
                    m.manager_m_view();
                    break;
                case 2:
                    system("CLS");
                    cout << "% Add Material" << endl;
                    m.manager_m_add(m_material);
                    goto m1_3;
                    break;
                case 3:
                    system("CLS");
                    cout << "% Edit Material" << endl;
                    m.manager_m_edit(m_material);
                    goto m1_3;
                    break;
                case 4:
                    system("CLS");
                    cout << "% Delete Material" << endl;
                    m.manager_m_delete(m_material);
                    goto m1_3;
                    break;
                case 5:
                    system("CLS");
                    cout << "% Return to the previous page!" << endl;
                    goto m1;
                    break;
                case 6:
                    system("CLS");
                    cout << "% Remove the main menu and log out!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please enter the numbers between 1 to 4!" << endl;
                    goto m1_3;
                    break;
                }
                goto m1;
                break;

            case 4://edit dish
                system("CLS");
                cout << "% Manage Dish\n" << endl;
            m1_4:
                int m_choice4;
                cout << "\n ********* Manager Menu - Dish *********" << endl;
                cout << " * 1: view dish                        * " << endl;
                cout << " * 2: edit dish                        * " << endl;
                cout << " * 3. delete dish                      * " << endl;
                cout << " * 4. return to the previous screen    * " << endl;
                cout << " * 5: quit and return main menu        *" << endl;
                cout << " ***************************************" << endl;
                cout << " Please input a number of your choice: ";
                cin >> m_choice4;
                switch (m_choice4) {
                case 1:
                    system("CLS");
                    cout << "% View dish" << endl;
                    m.manager_d_view();
                    goto m1_4;
                    break;
                case 2:
                    system("CLS");
                    cout << "% Edit dish" << endl;
                    View_menu();
                    m.manager_d_edit(m_dish);
                    goto m1_4;
                    break;
                case 3:
                    system("CLS");
                    cout << "% delete dish" << endl;
                    m.manager_d_delete(m_dish);
                    goto m1_4;
                    break;
                case 4:
                    system("CLS");
                    cout << "% Return to the previous page" << endl;
                    goto m1;
                    break;
                case 5:
                    system("CLS");
                    cout << "% Return to the main menu and log out!" << endl;
                    mainMenu();
                    return;
                default:
                    system("CLS");
                    cout << "Invalid choice! Please enter the numbers between 1 to 5!" << endl;
                    goto m1_4;
                    break;
                }
                goto m1;
                break;
            case 5:
                system("CLS");
                cout << "% View the menu for table" << endl;
                m.manager_view_table_menu();
                goto m1;
                break;
            case 6://check the payment situation
                system("CLS");
                cout << "% Check the payment situation" << endl;
                m.m_pay_bill();
                goto m1;
                break;
            case 7://total profit
                system("CLS");
                cout << "% Check the total profit at this time." << endl;
                m.manager_check_sum();
                goto m1;
                break;
            case 8:
                system("CLS");
                std::cout << "% Return main menu and log out!" << endl;
                return;
            default:
                system("CLS");
                std::cout << "Invaild input" << std::endl;
                goto m1;
                break;
            }
        }
    }

    void customerMenu(int user_id) {
        load();
        CustomerService* user_identity;
        if (user_id == -1) { user_identity = new CustomerService; }
        else { user_identity = &users[user_id]; }
        while (true) {
            std::cout << "\n ********** Customer Menu *********" << endl;
            std::cout << " * 1: view menu                   *" << endl;
            std::cout << " * 2: book an order               *" << endl;
            std::cout << " * 3: add some dishes             *" << endl;
            std::cout << " * 4: delete the dish             *" << endl;
            std::cout << " * 5: pay bill                    *" << endl;
            std::cout << " * 6: quit and return main menu   *" << endl;
            std::cout << " **********************************" << endl;
            std::cout << " Please input a number to run the program: ";
            int num_cu;
            std::cin >> num_cu;
            switch (num_cu) {
            case 1:
                //view menu function
                system("CLS");
                cout << "% View Menu" << endl;
                user_identity->view_menu();
                break;
            case 2:
                system("CLS");
                cout << "% Book an order" << endl;
                cout << "query product in cart function" << endl;
                user_identity->place_order();

                write();
                break;
            case 3:
                system("CLS");
                cout << "% Add dishes" << endl;
                user_identity->Addition();
                write();
                break;
            case 4:
                system("CLS");
                cout << "% Delete dish" << endl;
                user_identity->Decline();
                write();
                break;
            case 5:
                system("CLS");
                cout << "% Pay bill function" << endl;
                user_identity->pay_bill();
                write();
                break;
            case 6:
                system("CLS");
                std::cout << "% Return main menu and log out!" << endl;
                mainMenu();
                break;
            default:
                system("CLS");
                std::cout << "invaild input" << std::endl;
                break;
            }
        }
        delete user_identity;
    }
};

int main() {
    RestaurantManagementSystem a;
    a.mainMenu();
}
