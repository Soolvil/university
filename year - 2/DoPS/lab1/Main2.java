// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

public class Main2 {
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
      double a, b, z, x;
      
      Scanner input = new Scanner(System.in);
      String inputBuffer;
      inputBuffer = filteredScan(input,
	 "Введите параметр a:",
	 "Значение параметра должно быть дробным числом.\nВведите параметр a:",
	 (String str) -> parseableDouble(str));
      a = Double.parseDouble(inputBuffer);
      inputBuffer = filteredScan(input,
	 "Введите параметр b:",
	 "Значение параметра должно быть дробным числом.\nВведите параметр b:",
	 (String str) -> parseableDouble(str));
      b = Double.parseDouble(inputBuffer);
      while(b < a){
	 System.out.println("Параметр b должен быть не меньше параметра a.");
	 inputBuffer = filteredScan(input,
	    "Введите параметр b:",
	    "Значение параметра должно быть дробным числом.\nВведите параметр b:",
	    (String str) -> parseableDouble(str));
      }
      inputBuffer = filteredScan(input,
	 "Введите параметр z:",
	 "Значение параметра должно быть дробным числом.\nВведите параметр z:",
	 (String str) -> parseableDouble(str));
      z = Double.parseDouble(inputBuffer);
      inputBuffer = filteredScan(input,
	 "Введите аргумент x:",
	 "Значение аргумента должно быть дробным числом.\nВведите аргумент x:",
	 (String str) -> parseableDouble(str));
      x = Double.parseDouble(inputBuffer);
      input.close();
      
      if(x < a){
	 if(Math.abs(z / x) > 1.0){
	    System.out.println("При данных параметрах функция не определена.");
	 }
	 else {
	    System.out.println("Значение функции:" + Math.asin(z / x));
	 }
      }
      else if(x <= b){
	 System.out.println("Значение функции:" + (Math.exp(a * x) + b * x));
      }
      else {
	 if(Math.exp(x) == z){
	    System.out.println("При данных параметрах функция не определена.");
	 }
	 else {
	    System.out.println("Значение функции:" + (1 / (Math.exp(x) - z)));  
	 }
      }
   }
}
