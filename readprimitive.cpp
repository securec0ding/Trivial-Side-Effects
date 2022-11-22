#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

#define MAX_SCORES 10
#define MAX_USERS 10

class UserSession
{
public:
  UserSession()
  {
    Initialize();
  }

  bool IsActive() const
  {
    return isActive_;
  }

  void SetActive(bool isActive)
  {
    isActive_ = isActive;
    if (!isActive)
      Initialize();
  }

  auto GetScores()
  {
    return make_pair(begin(scores_), begin(scores_) + lastScoreIndex_);
  }

  void AddScore(int score)
  {
    if (++lastScoreIndex_ > MAX_SCORES)
    {
      return;
    }
    scores_[lastScoreIndex_] = score;
  }

private:
  void Initialize()
  {
    isActive_ = false;
    lastScoreIndex_ = 0;
    memset(scores_, 0, sizeof(scores_));
  }

private:
  bool isActive_;
  int lastScoreIndex_;
  int scores_[MAX_SCORES];
};

class User
{
public:
  User()
    : isValid_(false)
    , isSuperUser_(false)
  {}

  User(const string& username, const string& password, bool isSuperUser = false)
    : isValid_(true)
    , isSuperUser_(isSuperUser)
    , username_(username)
    , password_(password)
  {}

  bool IsValid() const
  {
    return isValid_;
  }

  bool IsSuperUser() const
  {
    return isSuperUser_;
  }

  bool CheckCredentials(const string& username, const string& pass) const
  {
    return username_ == username && password_ == pass;
  }

private:
  bool isValid_;
  bool isSuperUser_;
  string username_;
  string password_;
};

UserSession currentUser;
User registeredUsers[MAX_USERS] = {
  User("root", "p4s$", true)
};

bool authenticateUser(const string& username, const string& password)
{
  for (auto it = begin(registeredUsers); it != end(registeredUsers); it++)
  {
    if (it->CheckCredentials(username, password))
    {
      return true;
    }
  }
  return false;
}

bool registerUser(const string& username, const string& password)
{
  for (auto it = begin(registeredUsers); it != end(registeredUsers); it++)
  {
    if (!it->IsValid())
    {
      *it = User(username, password);
      return true;
    }
  }
  return false;
}

void runTheGame()
{
  const string questions[] = {
    "What is the total number of dots on a pair of dice?",
    "1+2 equals what?",
    "Which other planet is the closest to Earth?"
  };

  const string answers[] = {
    "42",
    "3",
    "Venus"
  };

  cin.ignore();

  int score = 0;
  for (size_t i = 0; i < sizeof(questions) / sizeof(questions[0]); i++)
  {
    cout << "Question " << i + 1 << ": " << questions[i] << endl;
    cout << "Your answer: ";

    string answer;
    getline(cin, answer);

    if (answer == answers[i])
    {
      cout << "Correct!" << endl;
      score++;
    }
    else
    {
      cout << "Incorrect!" << endl;
    }
  }

  cout << "You earned " << score << " point(s)." << endl;
  currentUser.AddScore(score);
}

int main(int argc, char* argv[])
{
  cout << argv[0] << " - the most performant Q&A game." << endl;
  while (true)
  {
    bool is_logged_in = currentUser.IsActive();
    if (is_logged_in)
    {
      cout << "(x) - logout" << endl
         << "(s) - start the game" << endl
         << "(p) - get my game scores" << endl;
    }
    else
    {
      cout << "(l) - log in" << endl
         << "(r) - register" << endl;
    }

    cout << "(q) - quit game." << endl;

    string command;
    cin >> command;
    if (command == "l" && !is_logged_in)
    {
      string username, password;
      cout << "To log in, please enter your username: ";
      cin >> username;
      cout << "Password: ";
      cin >> password;

      if (authenticateUser(username, password))
      {
        currentUser.SetActive(true);
        cout << "Successfuly authenticated as " << username << endl;
      }
      else
      {
        cout << "Authentication failed." << endl;
      }
    }
    else if (command == "r" && !is_logged_in)
    {
      string username, password;
      cout << "To register a new account, enter your username: ";
      cin >> username;
      cout << "Password: ";
      cin >> password;

      if (registerUser(username, password))
      {
        cout << "Your account is registered! Now you may log in." << endl;
      }
      else
      {
        cout << "Registration failed; no more than " << MAX_USERS << " accounts are allowed." << endl;
      }
    }
    else if (command == "x" && is_logged_in)
    {
      currentUser.SetActive(false);
    }
    else if (command == "s" && is_logged_in)
    {
      runTheGame();
    }
    else if (command == "p" && is_logged_in)
    {
      cout << "Your scores are: " << endl;
      auto scores_iterators = currentUser.GetScores();
      for (auto it = scores_iterators.first; it != scores_iterators.second; it++)
      {
        cout << *it << endl;
      }
    }
    else if (command == "q")
    {
      cout << "Exiting..." << endl;
      break;
    }
  }
}