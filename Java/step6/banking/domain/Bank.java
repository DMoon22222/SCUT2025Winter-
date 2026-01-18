package banking.domain;

public class Bank
{
    private int numberOfCustomers;
    private Customer[] customers;
    private static Bank instance;
    public Bank()
    {
        numberOfCustomers = 0;
        customers = new Customer[10];
    }
    public void addCustomer(String firstName, String lastName)
    {
        customers[numberOfCustomers] = new Customer(firstName, lastName);
        numberOfCustomers++;
    }
    public int getNumOfCustomers()
    {
        return numberOfCustomers;
    }
    public Customer getCustomer(int index)
    {
        return customers[index];
    }
    public static Bank getBank()
    {
        if(instance==null) {
            instance = new Bank();
        }
        return instance;
    }
}
