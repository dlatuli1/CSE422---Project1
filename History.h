#include <string>
#include <vector>
#include <deque>

using namespace std;

class History
{
public:
   // ************************
   // *** Public functions ***
   // ************************

   // Constructor
   History();

   // Deconstructor
   ~History();

   // History Size
   unsigned int HistorySize();

   // Trim Old
   void TrimOld();

   // Push New
   void PushNew(string command);

   string Get(size_t location);

private:   
   // **********************
   // *** Private fields ***
   // **********************

   deque<string> commandHistory;
};
