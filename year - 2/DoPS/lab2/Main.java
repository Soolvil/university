// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

public class Main {  
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
      System.out.println("Введите серию чисел, чтобы завершить ввод введите 0.");
      int proofingHumidity;
      
      final int minOptimal = 75;
      final int maxOptimal = 85;
      final int lowCritical = 65;
      final int highCritical = 95;

      int lowHumidityCount = 0;
      int highHumidityCount = 0;
      int criticalHumidityCount = 0;
      int restorationCount = 0;
      boolean quickRestoration = false;

      boolean wasCritical = false;
      int lastCritical = 0;

      Scanner input = new Scanner(System.in);
      String inputBuffer;
      while(true){
	 inputBuffer = filteredScan(input, 
	    "Введите целое число процентов:",
	    "Данные должны быть целым чиcлом от 0 до 100.\nВведите ещё раз:",
	    (String str) -> parseableInt(str, (Integer i) -> i <= 100 && i >= 0));
	 proofingHumidity = Integer.parseInt(inputBuffer);
	 if(proofingHumidity == 0)break;
	 if(proofingHumidity >= minOptimal && proofingHumidity <= maxOptimal && wasCritical){
	    restorationCount++;
	    wasCritical = false;
	    if(lastCritical <= 5){
	       quickRestoration = true;
	    }
	 }
	 else if(proofingHumidity < minOptimal && proofingHumidity >= lowCritical){
	    lowHumidityCount++;
	 }
	 else if(proofingHumidity > maxOptimal && proofingHumidity <= highCritical){
	    highHumidityCount++;
	 }
	 else if(proofingHumidity < lowCritical || proofingHumidity > highCritical){
	    criticalHumidityCount++;
	    wasCritical = true;
	    lastCritical = 0;
	 }
	 lastCritical++;
      }
      System.out.println("Подсыханий: " + lowHumidityCount 
	 + ", Переувлажнений: " + highHumidityCount
	 + ", Критических ситуаций: " + criticalHumidityCount
	 + ", Восстановлений: " + restorationCount
	 + ", Резкое восстановление: " + quickRestoration);
   }
}
