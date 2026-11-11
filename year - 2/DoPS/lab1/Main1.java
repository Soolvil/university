// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

public class Main1 {
   public static double calcMonthPay(double reading, double tariff, double socialNorm, int month){
      double resultingPay = 0;
      resultingPay += reading;
      if(reading > socialNorm){
	 resultingPay += (reading - socialNorm) * 0.5;
      }
      resultingPay *= tariff;
      if(month == 12 || month == 1 || month == 2){
	 resultingPay *= 1.1;
      }
      return resultingPay;
   }
   
   public static boolean parseableInt(String str){
      try {
	 Integer.parseInt(str);
	 return true;
      }
      catch (NumberFormatException e){
	 return false;
      }
   }
   
   public static boolean parseableInt(String str, Predicate<Integer> filter){
      try {
	 return filter.test(Integer.parseInt(str));
      }
      catch (NumberFormatException e){
	 return false;
      }
   }

   public static boolean parseableDouble(String str){
      try {
	 Double.parseDouble(str);
	 return true;
      }
      catch (NumberFormatException e){
	 return false;
      }
   }

   public static boolean parseableDouble(String str, Predicate<Double> filter){
      try {
	 return filter.test(Double.parseDouble(str));
      }
      catch (NumberFormatException e){
	 return false;
      }
   }

   public static String filteredScan(Scanner scan, String scanMsg, String failMsg, Predicate<String> filter){
      String buffer;
      System.out.print(scanMsg);
      buffer = scan.nextLine().trim();
      while(!filter.test(buffer)){
	 System.out.print(failMsg);
	 buffer = scan.nextLine().trim();
      };
      return buffer;
   }

   public static void main(){
      double currentReading, previousReading;
      boolean hasElectricStove;
      int residents;
      int currentMonth;

      Scanner input = new Scanner(System.in);
      String inputBuffer;
      inputBuffer = filteredScan(input, 
	 "Показания счётчика за текущий месяц:", 
	 "Показания счётчика должны быть неотрицательным числом.\nВведите данные ещё раз:",
	 (String str) -> parseableDouble(str, (Double d) -> d >= 0.0));
      currentReading = Double.parseDouble(inputBuffer);
      inputBuffer = filteredScan(input, 
	 "Показания счётчика за предыдущий месяц:", 
	 "Показания счётчика должны быть неотрицательным числом.\nВведите данные ещё раз:",
	 (String str) -> parseableDouble(str, (Double d) -> d >= 0.0));
      previousReading = Double.parseDouble(inputBuffer);
      inputBuffer = filteredScan(input, 
	 "Есть ли электрическая плита:", 
	 "Значения должно быть true или false.\nВведите данные ещё раз:",
	 (String str) -> str.equalsIgnoreCase("true") || str.equalsIgnoreCase("false"));
      hasElectricStove = Boolean.parseBoolean(inputBuffer);
      inputBuffer = filteredScan(input, 
	 "Количество проживающих:", 
	 "Значение должно быть целым неотрицательным числом.\nВведите данные ещё раз:",
	 (String str) -> parseableInt(str, (Integer i) -> i >= 0));
      residents = Integer.parseInt(inputBuffer);
      inputBuffer = filteredScan(input,
	 "Введите текущий месяц:",
	 "Месяц должен быть целым числом от 1 до 12.\nВведите данные ещё раз:",
	 (String str) -> parseableInt(str, (Integer i) -> i >= 1 && i <= 12));
      currentMonth = Integer.parseInt(inputBuffer);
      input.close();
      
      final double socialNorm = residents * (residents > 3 ? 150 : 100);
      final double tariff = (hasElectricStove ? 4 : 5);
      final int previousMonth = (currentMonth == 1 ? 12 : currentMonth - 1);
      
      final double previousMonthPay = calcMonthPay(previousReading, tariff, socialNorm, previousMonth);
      final double currentMonthPay = calcMonthPay(currentReading, tariff, socialNorm, currentMonth);
      
      final double finalPay = previousMonthPay + currentMonthPay + (previousMonthPay > 500 ? 100 : 0);
      System.out.println("Сумма к оплате: " + finalPay + " руб.");
   }
}
