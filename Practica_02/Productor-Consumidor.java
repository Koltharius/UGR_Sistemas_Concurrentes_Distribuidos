import monitor.*;

class Buffer extends AbstractMonitor {
//Variables
private int numSlot=0, cont=0;
private int buffer[] = null;
private Condition produciendo = makeCondition();
private Condition consumiendo = makeCondition();

//Metodos
public Buffer(int p_numSlot)
{
        numSlot = p_numSlot;
        buffer = new int[numSlot];
}

public void depositar(int valor) throws InterruptedException
{
        enter();

        while(cont == numSlot)
        {
                produciendo.await();
        }

        buffer[cont]=valor;
        cont++;
        if(cont == numSlot)
        {
                consumiendo.signal();
        }
        leave();
}

public int extraer() throws InterruptedException
{
        enter();

        int valor;
        while(cont==0)
        {
                consumiendo.await();
        }

        cont--;
        valor=buffer[cont];

        if(cont == 0)
        {
                produciendo.signal();
        }

        leave();
        return valor;
}
}

class Productor implements Runnable {
//Variables
private Buffer bb;
int veces, numP;
Thread thr;

//Metodos
public Productor(Buffer pbb, int pveces, int pnumP)
{
        bb = pbb;
        veces = pveces;
        numP = pnumP;
        thr = new Thread(this,"El Productor " + numP);
}

public void run()
{
        try
        {
                int item = 100*numP;

                for(int i=0; i<veces; i++)
                {
                        System.out.println(thr.getName()+", produce "+item);
                        bb.depositar(item++);
                }
        }

        catch(Exception e)
        {
                System.err.println("Exepción en main: "+e);
        }
}
}

class Consumidor implements Runnable
{
//Variables
private Buffer bb;
int veces, numC;
Thread thr;

//Metodos
public Consumidor(Buffer pbb, int pveces, int pnumC)
{
        bb = pbb;
        veces = pveces;
        numC = pnumC;
        thr = new Thread(this, "El Consumidor "+numC);
}

public void run()
{
        try
        {
                for(int i=0; i<veces; i++)
                {
                        int item = bb.extraer();
                        System.out.println(thr.getName()+", consume "+item);
                }
        }
        catch(Exception e)
        {
                System.err.println("Exepción en consumidor: "+e);
        }
}
}

class prodconsum {
public static void main(String[] args)
{
        if(args.length != 5)
        {
                System.err.println("Uso: ncons nprod tambuf niterp niterc");
                return;
        }

        // leer parametros, crear vectores y buffer intermedio
        Consumidor[] cons = new Consumidor[Integer.parseInt(args[0])];
        Productor[] prod = new Productor[Integer.parseInt(args[1])];
        Buffer buffer = new Buffer(Integer.parseInt(args[2]));
        int iter_cons = Integer.parseInt(args[3]);
        int iter_prod = Integer.parseInt(args[4]);

        if(cons.length*iter_cons != cons.length*iter_prod)
        {
                System.err.println("No coinide el número de items a producir con el número de items a consumir");
                return;
        }

        //Crear hebras
        for(int i=0; i<cons.length; i++)
        {
                cons[i] = new Consumidor(buffer, iter_cons, i);
        }

        for(int i=0; i<prod.length; i++)
        {
                prod[i] = new Productor(buffer, iter_prod, i);
        }

        //poner en marcha las hebras
        for(int i=0; i<prod.length; i++)
        {
                prod[i].thr.start();
        }

        for(int i=0; i<cons.length; i++)
        {
                cons[i].thr.start();
        }
}
}
