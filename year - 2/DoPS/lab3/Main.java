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
      int arrSize;
      int[] arr;

      Scanner input = new Scanner(System.in);
      String inputBuffer;
      inputBuffer = filteredScan(input,
	 "Введите размер массива:",
	 "Размер массива должен быть целым чётным неотрицательным числом.\nВведите ещё раз:",
	 (String str) -> parseableInt(str, (Integer i) -> i >= 0 && i % 2 == 0));
      arrSize = Integer.parseInt(inputBuffer);
      arr = new int[arrSize];
      int lowCount = 0, highCount = 0;
      do{
	 if(lowCount != highCount){
	    System.out.println("В массиве должно быть одинаковое количество элементов в промежутках [0;50] и (50; 100]");
	    System.out.println("Введите массив заново.");
	 }
	 lowCount = 0; highCount = 0;
	 for(int i = 0; i < arrSize; i++){
	    inputBuffer = filteredScan(input,
	       "Введите элемент массива:",
	       "Элементы массива должны быть целыми числам в промежутке [0;100].\nВведите ещё раз:",
	       (String str) -> parseableInt(str, (Integer num) -> num >= 0 && num <= 100));
	    arr[i] = Integer.parseInt(inputBuffer);
	    if(arr[i] <= 50)lowCount++;
	    else highCount++;
	 }
      } while(lowCount != highCount);
      System.out.println("Изначальный массив:");
      for(int i : arr){
	 System.out.print(i + " ");
      }
      System.out.print("\n");
      int lowIter = 0, highIter = 0;
      while(lowIter < arrSize && highIter < arrSize){
	 while(lowIter < arrSize && arr[lowIter] > 50){
	    lowIter++;
	 }
	 while(highIter < arrSize && arr[highIter] <= 50){
	    highIter++;
	 }
	 if(lowIter >= arrSize || highIter >= arrSize)break;
	 int temp = arr[lowIter];
	 arr[lowIter] = arr[highIter];
	 arr[highIter] = temp;
      }
      System.out.println("Итоговый массив:");
      for(int i : arr){
	 System.out.print(i + " ");
      }
      System.out.print("\n");
   }
}
