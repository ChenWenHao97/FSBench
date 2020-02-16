#include<iostream>
using namespace std;
class Singleon
{
private:
	Singleon()
	{
		cout << "Singleon()" << endl;
	}
	static Singleon* instance;
public:
	static Singleon* GetSingleon()
	{
		return instance;
	}
	static Singleon* Destroy()
	{
		delete instance;
		instance = NULL;
	}
};
Singleon* Singleon::instance = new Singleon();
int main()
{
	Singleon* sl1 = Singleon::GetSingleon();
	Singleon* sl2 = Singleon::GetSingleon();
	Singleon* sl3 = Singleon::GetSingleon();
	cout << sl1 << endl;
	cout << sl2 << endl;
	cout << sl2 << endl;
	system("pause");
	return 0;

}