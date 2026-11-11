// Вариант 21
import java.util.Scanner;
import java.util.function.Function;
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
   
   public static int calcStandartFare(int hours){
      return Math.min(hours, 3) * 100;
   }

   public static int calcExtendedFare(int hours){
      return Math.max(hours - 3, 0) * 50;
   }

   public static int calcFare(int hours){
      return calcStandartFare(hours) + calcExtendedFare(hours);
   }

   public static int[] calcTotalRevenue(int[] arr){
      int[] res = new int[arr.length];
      for(int i = 0; i < arr.length; i++){
	 res[i] = calcFare(arr[i]);
      }
      return res;
   }

   public static void main(){
      Scanner input = new Scanner(System.in);
      String inputBuffer;
      int arrSize;
      inputBuffer = filteredScan(input,
	 "Введите размер массива:",
	 "Размер массива должен быть целым неотрицательным числом.\nВведите ещё раз:",
	 (String str) -> parseableInt(str, (Integer num) -> num >= 0));
      arrSize = Integer.parseInt(inputBuffer);
      int[] parkingHours = new int[arrSize];
      for(int i = 0; i < parkingHours.length; i++){
	 inputBuffer = filteredScan(input,
	    "Введите количество часов для " + (i + 1) + "-ой машины:",
	    "Количество часов должно быть целым неотрицательным числом.\nВведите ещё раз:",
	    (String str) -> parseableInt(str, (Integer num) -> num >= 0));
	 parkingHours[i] = Integer.parseInt(inputBuffer);
      }
      input.close();
      
      System.out.print("Количество часов:\n");
      for(int i : parkingHours)System.out.print(i + " ");
      System.out.println();

      int[] totalRevenue = calcTotalRevenue(parkingHours);
      System.out.print("Выручка с машины:\n");
      for(int i : totalRevenue)System.out.print(i + " ");
      System.out.println();
   }
}
