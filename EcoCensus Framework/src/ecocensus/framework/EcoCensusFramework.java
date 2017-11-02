/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ecocensus.framework;
import java.io.*;
/**
 *
 * @author mfriend
 */
public class EcoCensusFramework {
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        try {
            String prg = "import sys\nprint int(sys.argv[1])+int(sys.argv[2])\n";
            BufferedWriter out = new BufferedWriter(new FileWriter("test1.py"));
            out.write(prg);
            out.close();
            int number1 = 10;
            int number2 = 32;
            
            ProcessBuilder pb = new ProcessBuilder(
                    "C:\\Python27\\python", "test1.py", ""+number1, ""+number2);
            Process p = pb.start();
        
            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
            int ret = new Integer(in.readLine()).intValue();
            System.out.println("value is : "+ret);
        }catch(Exception e){
            System.out.println(e);
        }
    
    }
    
}

