import monitor.*;
import java.util.Random;
import java.util.ArrayList;

class Estanco extends AbstractMonitor
{
private ArrayList<String> ingredientes = new ArrayList<String>();
private ArrayList<Condition> fumadores = new ArrayList<Condition>();
private Condition estanquero = makeCondition();
//private Condition fumador = makeCondition();
private int estado;

public Estanco()
{
        estado = 3;
        ingredientes.add("Papel");
        ingredientes.add("Tabaco");
        ingredientes.add("Cerillas");

        for(int i=0; i<3; i++)
        {
                Condition f = makeCondition();
                fumadores.add(f);
        }
}

public void obtenerIngrediente (int miIngrediente)
{
        enter();
        if(estado != miIngrediente)
                fumadores.get(miIngrediente).await();

        System.out.println("Se ha desbloqueado el fumador "+miIngrediente);
        estado=3;
        estanquero.signal();
        leave();
}

public void ponerIngrediente(int ingrediente)
{
        enter();

        if(estado == 3)
        {
                System.out.println(ingredientes.get(ingrediente)+" ha sido repuesto.");
                estado = ingrediente;

                if(ingrediente == 0)
                {
                        fumadores.get(0).signal();
                }
                else if(ingrediente == 1)
                {
                        fumadores.get(1).signal();
                }

                else
                {
                        fumadores.get(2).signal();
                }
        }
        leave();
}

public void esperarRecogidaIngrediente()
{
        enter();

        System.out.println("El estanquero esta esperando a que el ingrediente sea retirado.");
        int i=0;
        if(estado != 3)
        {
                estanquero.await();
        }

        leave();
}
}

class Fumador implements Runnable
{
int miIngrediente;
public Thread thr;
private Estanco estanco;

public Fumador(int p_miIngrediente, Estanco es)
{
        miIngrediente = p_miIngrediente;
        estanco = es;
        thr = new Thread(this,"Fumador"+miIngrediente);
}

public void run()
{
        while(true)
        {
                estanco.obtenerIngrediente(miIngrediente);
                System.out.println("Soy el fumador "+miIngrediente+" y empiezo a fumar");
                aux.dormir_max(2000);
                System.out.println("Soy el fumador "+miIngrediente+" y dejo de fumar");
        }
}
}

class Estanquero implements Runnable
{
public Thread thr;
private Estanco estanco;

public Estanquero(Estanco es)
{
        estanco = es;
        thr = new Thread(this,"Estanquero");
}

public void run()
{
        int ingredientes;

        while(true)
        {
                ingredientes = (int) (Math.random() * 3.0);
                estanco.ponerIngrediente(ingredientes);
                estanco.esperarRecogidaIngrediente();
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

        catch(InterruptedException e)
        {
                System.err.println("sleep interrumpido en 'aux.dormir_max()'");
        }
}
}

class fumadores
{
public static void main(String[] args)
{
        Estanco es = new Estanco();
        Fumador[] fum = new Fumador[3];

        //Creacion de la hebra del estanquero
        Estanquero est = new Estanquero(es);

        //Creacion de las hebras de los fumadores
        for(int i=0; i<3; i++)
        {
                fum[i] = new Fumador(i,es);
        }

        //Ponemos las hebras en marcha
        est.thr.start();

        for(int i=0; i<3; i++)
        {
                fum[i].thr.start();
        }
}
}
