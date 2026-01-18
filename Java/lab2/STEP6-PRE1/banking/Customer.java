package banking;

public class Customer {
    private String firstName;
    private String lastName;
    private Account[] accounts;
    private int numberOfAccounts;

    public Customer(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.accounts = new Account[10];
        this.numberOfAccounts = 0;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName() {
        return lastName;
    }
    public void addAccount(Account account) {
        if (numberOfAccounts < accounts.length) {
            accounts[numberOfAccounts] = account;
            numberOfAccounts++;
        }
    }

    public Account getAccount(int index) {
        if (index >= 0 && index < numberOfAccounts) {
            return accounts[index];
        }
        return null;
    }

    public int getNumOfAccounts() {
        return numberOfAccounts;
    }
    public void setAccount(Account account) {
        if (numberOfAccounts == 0) {
            addAccount(account);
        } else {
            accounts[0] = account;
        }
    }
    public Account getAccount() {
        if (numberOfAccounts > 0) {
            return accounts[0];
        }
        return null;
    }
}