#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

using namespace std;

// Абстрактный класс для пользователей
class AbstractUser {
public:
    virtual void displayInfo() const = 0; // Чисто виртуальная функция
    virtual ~AbstractUser() = default;
};

// Базовый класс User
class User : public AbstractUser {
protected:
    int id;                        // ID пользователя
    std::string name;              // Имя пользователя
    std::string username;          // Логин
    double balance;                // Баланс
    static int userCount;          // Статическое поле: количество пользователей

public:
    User(int userId, const std::string& userName, const std::string& userUsername, double userBalance)
        : id(userId), name(userName), username(userUsername), balance(userBalance) {
        ++userCount;
    }

    // Виртуальный метод для отображения информации
    virtual void displayInfo() const override {
        cout << "ID: " << id << ", Имя: " << name << ", Логин: " << username
            << ", Баланс: " << balance << " руб.\n";
    }

    virtual ~User() { --userCount; }

    // Перегрузка оператора присваивания
    User& operator=(const User& other) {
        if (this == &other) return *this;
        id = other.id;
        name = other.name;
        username = other.username;
        balance = other.balance;
        return *this;
    }

    // Методы для работы с балансом
    void addBalance(double amount) {
        balance += amount;
    }

    void subtractBalance(double amount) {
        if (balance < amount)
            throw std::logic_error("Недостаточно средств!");
        balance -= amount;
    }

    double getBalance() const {
        return balance;
    }

    static int getUserCount() {
        return userCount;
    }
};
int User::userCount = 0;

// Производный класс PremiumUser
class PremiumUser : public User {
private:
    double bonusBalance; // Дополнительный бонусный баланс

public:
    PremiumUser(int userId, const std::string& userName, const std::string& userUsername, double userBalance, double bonus)
        : User(userId, userName, userUsername, userBalance), bonusBalance(bonus) {}

    // Переопределение метода с вызовом базового
    void displayInfo() const override {
        User::displayInfo(); // Вызов базового метода
        cout << "Бонусный баланс: " << bonusBalance << " руб.\n";
    }

    void useBonus(double amount) {
        if (bonusBalance < amount)
            throw std::logic_error("Недостаточно бонусов!");
        bonusBalance -= amount;
    }

    double getBonusBalance() const {
        return bonusBalance;
    }
};

// Класс Admin
class Admin : public User {
private:
    std::string accessLevel; // Уровень доступа администратора

public:
    Admin(int userId, const std::string& userName, const std::string& userUsername, double userBalance, const std::string& access)
        : User(userId, userName, userUsername, userBalance), accessLevel(access) {}

    void displayInfo() const override {
        cout << "[Admin] ";
        User::displayInfo();
        cout << "Уровень доступа: " << accessLevel << endl;
    }
};

// Класс Game
class Game {
private:
    std::string gameName;
    double minBet;
    double maxBet;
    double payoutMultiplier;

public:
    Game(const std::string& name, double min, double max, double multiplier)
        : gameName(name), minBet(min), maxBet(max), payoutMultiplier(multiplier) {}

    const std::string& getGameName() const {
        return gameName;
    }

    double calculatePayout(double betAmount, bool isWin) const {
        return isWin ? betAmount * payoutMultiplier : 0;
    }

    void displayInfo() const {
        cout << "Игра: " << gameName << ", Мин. ставка: " << minBet << " руб., Макс. ставка: " << maxBet
            << " руб., Множитель: " << payoutMultiplier << "x\n";
    }
};

// Класс Transaction
class Transaction {
private:
    int id;
    int userId;
    double amount;
    std::string timestamp;

public:
    Transaction(int transId, int usrId, double transAmount, const std::string& time)
        : id(transId), userId(usrId), amount(transAmount), timestamp(time) {}

    void displayInfo() const {
        cout << "Транзакция #" << id << ", Пользователь ID: " << userId << ", Сумма: " << amount
            << " руб., Время: " << timestamp << endl;
    }
};

// Класс GameRound
class GameRound {
private:
    User* player;
    Game* game;
    double betAmount;
    bool isWin;

public:
    GameRound(User* p, Game* g, double bet, bool result)
        : player(p), game(g), betAmount(bet), isWin(result) {}

    void playRound() {
        if (isWin) {
            double payout = game->calculatePayout(betAmount, isWin);
            player->addBalance(payout);
            cout << "Выигрыш: " << payout << " руб.\n";
        }
        else {
            player->subtractBalance(betAmount);
            cout << "Проигрыш: " << betAmount << " руб.\n";
        }
    }

    void displayResult() const {
        cout << "Игрок: " << player->getBalance() << ", Игра: " << game->getGameName()
            << ", Ставка: " << betAmount << " руб., Результат: " << (isWin ? "Выигрыш" : "Проигрыш") << endl;
    }
};

// Класс Jackpot
class Jackpot {
private:
    double currentAmount;
    bool isWon;

public:
    Jackpot(double amount) : currentAmount(amount), isWon(false) {}

    void tryWin(User& user) {
        if (!isWon) {
            isWon = true;
            user.addBalance(currentAmount);
            cout << "Джекпот сорван! Сумма: " << currentAmount << " руб.\n";
        }
        else {
            cout << "Джекпот уже был сорван.\n";
        }
    }

    void displayInfo() const {
        cout << "Джекпот: " << currentAmount << " руб., Состояние: "
            << (isWon ? "Сорван" : "Не сорван") << endl;
    }
};

// Класс Bonus
class Bonus {
private:
    std::string type;
    double value;

public:
    Bonus(const std::string& t, double v) : type(t), value(v) {}

    void apply(User& user) {
        user.addBalance(value);
        cout << "Бонус '" << type << "' добавил " << value << " руб. к балансу.\n";
    }

    void displayInfo() const {
        cout << "Бонус: " << type << ", Сумма: " << value << " руб.\n";
    }
};

// Основная функция
int main() {
    setlocale(LC_ALL, "rus");

    User user1(1, "Иван", "ivan123", 100.0);
    PremiumUser user2(2, "Мария", "maria456", 200.0, 50.0);
    Admin admin(0, "Админ", "admin", 1000.0, "Full");

    Game game("Рулетка", 10.0, 500.0, 2.0);
    Jackpot jackpot(5000.0);
    Bonus bonus("Фриспины", 20.0);

    game.displayInfo();
    user1.displayInfo();
    user2.displayInfo();
    admin.displayInfo();

    GameRound round(&user1, &game, 50.0, true);
    round.playRound();
    round.displayResult();

    bonus.apply(user2);
    jackpot.tryWin(user2);

    return 0;
}
