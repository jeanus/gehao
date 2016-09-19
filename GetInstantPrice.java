package apis;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;

public class GetInstantPrice {
	
	public static ArrayList<String> list = new ArrayList<String>();
	public static void readList(){
		try {
		String listAddr = "C:\\Users\\jeanus\\Desktop\\list.txt";
		File f=new File(listAddr);    
        InputStreamReader isr1=new InputStreamReader(new FileInputStream(f));  
        BufferedReader br=new BufferedReader(isr1);         
    	String s;
		while ((s = br.readLine()) != null) {
			if(s.trim().startsWith("6")){
				list.add("sh"+s.trim());
			}
			if(s.trim().startsWith("0")){
				list.add("sz"+s.trim());
			}
		}
		br.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	public static void main(String[] args){
		   
	        try { 
	        	readList();
	            for(String symbol : list){
	            	String res = "";   
		        	URL url = new URL("http://hq.sinajs.cn/list="+symbol);  
		            java.net.HttpURLConnection conn = (java.net.HttpURLConnection)url.openConnection();  
		            conn.setDoOutput(true);  
		            conn.setRequestMethod("POST");  
		            java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(conn.getInputStream(),"UTF-8"));  
		            String line;  
		            while ((line = in.readLine()) != null) {  
		                res += line;  
		            }  
		            in.close();  
		            System.out.println( symbol.substring(2)+"\t"+res.split(",")[3]);  
	            }
	        } catch (Exception e) {  
	            System.out.println("error in wapaction,and e is " + e.getMessage());  
	        }  	     
	       
		
	}

}
