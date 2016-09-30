import monitor.*;
import java.util.Random;
import java.util.ArrayList;

class Barberia extends AbstractMonitor
{
private Condition barbero = makeCondition();
private ArrayList<Condition> cliente = new ArrayList<Condition> ();
private int cola_clientes;    //en principio no hay clientes
private int num_clientes;
private int siguiente;

public Barberia(int n)
{
        for(int i=0; i<n; i++)
        {
                Condition c = makeCondition();
                cliente.add(c);
        }
        cola_clientes = n;
        num_clientes = n;
}

//invocado por los clientes para cortarse el pelo
public void cortarPelo (int num_cliente)
{
        enter();

        // Los clientes esperan a que el barbero termine con el cliente anterior
        // o bien, si no hay clientes, despierta al barbero

        System.out.println("El cliente "+num_cliente+" quiere que le corten el pelo");

        if(cola_clientes < num_clientes) //El barbero esta pelando a otro
        {
                cliente.get(num_cliente).await();
        }

        else if(cola_clientes == num_clientes) //el barbero esta dormido
        {
                cola_clientes--;
                barbero.signal();
                cliente.get(num_cliente).await();
        }

        leave();
}

// invocado por el barbero para esperar (si procede) a un nuevo cliente
// y sentarlo para el corte
public void siguienteCliente (int siguiente)
{
        enter();

        //El barbero espera la llegada de un cliente para servirlo
        if(cola_clientes == num_clientes)
        {
                barbero.await();
        }

        else
        {
                System.out.println("El barbero llama al cliente " + siguiente);
                cliente.get(siguiente).signal();
        }

        leave();
}

//invocado por el barbero para indicar que ha terminado de cortar el pelo
public void finCliente (int siguiente)
{
        enter();

        // El barbero le indica al cliente que puede salir de la barberia
        // esperando a que lo haga y entre otro cliente.
        cliente.get(siguiente).signal();
        System.out.println("Al cliente "+(siguiente)+" ya le han cortado el pelo");
        System.out.println("El barbero termina de cortarle el pelo al cliente "+(siguiente));
        cola_clientes++;

        if(cola_clientes == num_clientes)
        {
                barbero.await();
        }

        else
        {
                cliente.get(siguiente).signal();
        }

        leave();
}
}

class Cliente implements Runnable
{
public Thread thr;
private Barberia barberia;
private int num_cliente;

public Cliente(Barberia barb, int pnum_cliente)
{
        barberia = barb;
        num_cliente = pnum_cliente;
        thr = new Thread(this,"cliente"+num_cliente);
}

public void run()
{
        while(true)
        {
                //System.out.println("El cliente "+num_cliente+" quiere que le corten el pelo");
                barberia.cortarPelo(num_cliente); //el cliente espera (si procede) y se corta el pelo
                //System.out.println("Al cliente "+num_cliente+" ya le han cortado el pelo");
                aux.dormir_max(2000); //el cliente esta fuera de la barberia un tiempo
        }
}
}

class Barbero implements Runnable
{
public Thread thr;
private Barberia barberia;
private int num_clientes, siguiente;

public Barbero (Barberia barb, int p_num_clientes)
{
        barberia = barb;
        thr = new Thread(this,"barbero");
        num_clientes = p_num_clientes;
        siguiente = 0;
}

public void run()
{
        while (true)
        {
                if(siguiente < (num_clientes - 1))
                        siguiente++;

                else
                        siguiente = 0;

                //System.out.println("El barbero llama a un cliente");
                barberia.siguienteCliente(siguiente);
                //System.out.println("El barbero esta cortando el pelo a un cliente");
                aux.dormir_max(2500); //El barbero esta cortando el pelo
                //System.out.println("El barbero termina de cortarle el pelo a un cliente");
                barberia.finCliente(siguiente);
        }
}
}

class aux
{
static Random genAlea = new Random();
static void dormir_max(int milisecsMax)
{
        try
        {
                Thread.sleep(genAlea.nextInt(milisecsMax));
        }

        catch (InterruptedException e)
        {
                System.err.println("sleep interrumpido en 'aux.dormir_max'");
        }
}
}

class barberoqueduerme
{
public static void main(String[] args)
{
        if(args.length != 1)
        {
                System.out.println("Uso: num_clientes");
                return;
        }

        int num_clientes = Integer.parseInt(args[0]);
        Barberia barberia = new Barberia (num_clientes);
        Cliente[] c = new Cliente[num_clientes];

        //Creamos hebras
        for(int i=0; i<num_clientes; i++)
        {
                c[i] = new Cliente(barberia,i);
        }

        Barbero b = new Barbero(barberia, num_clientes);

        //ponemos en marcha las hebras
        b.thr.start();

        for(int i=0; i<num_clientes; i++)
        {
                c[i].thr.start();
        }
}
}
