/*
 * HX711.c
 *
 *  Created on: 7 abr. 2023
 *      Author: S4IOT
 */
#include "HX711.h"
//si ves este comentario, quiere decir que es la version donde le modifique a la logica de las pruebas de lectura y salida en caso de fallo

//Datos ADC
//#define CERO_FIJO   (8497829)

uint32_t adc_cero = 0;
uint32_t adc_peso1 = 0;
uint32_t adc_peso2 = 0;
uint32_t adc_peso3 = 0;

uint32_t adc_peso4 =0;
//uint32_t adc_peso5 =0;
//uint32_t adc_peso6 =0;
//uint32_t adc_peso7 =0;
//float diferencia = 0;

//DATOS PESOS
float peso_cero =0.0f;
float peso1 =5000.0f;
float peso2 =10000.0f;//conocidos

float peso3 =15000.0f;
float peso4 =20000.0f;
//uint32_t extra =10000;
uint32_t extra =0;


float var_ref=0.0f;

#define LIMITWAITINGTIME    (450000)

port_gpio_t DT_input_signal;
port_gpio_t SCK_output_signal;
gain_options gane_choose = 1;

void set_Gain(gain_options gain){
    uint32_t data;
    if(gain==gain_128){
        gane_choose =1;
    }else if(gain ==gain_64){
        gane_choose =2;
    }else{ //caso de 32 gain
        gane_choose = 3;
    }

    hx711_read(&data); //hacemos una lectura para adapatar a la ganancia

}


void hx711_init(Comunication_pins pin_set){ //esto va a ir hasta el final depues de haber validado lo de i2d, debe ser lo ultimo
  DT_input_signal = pin_set.input_Data;     //se define la entrada
    SCK_output_signal = pin_set.output_signal;       // aquiel pin de salida que es el reloj, lo declaramos

    GPIO_Initialization(DT_input_signal, Port_Input);
    GPIO_Initialization(SCK_output_signal, Port_Output);

    hx711_deinit(); //lo mantenemos en 1

}

void hx711_start(void){
    GPIO_Write(SCK_output_signal, 0);
}

void hx711_deinit(void){
    GPIO_Write(SCK_output_signal, 1); //lo dejamos encendido para que permanesca en off el modulo
    Delay(100000);
}

uint8_t read_input_state(void){

    uint8_t input_state=0;
    GPIO_Read(DT_input_signal, &input_state);
    return input_state;
}

general_codes_t hx711_read(uint32_t* data_out){

//    general_codes_t status=successful_task;
    uint32_t data =0; //aqui va a estar el valor
    uint8_t input_value_state = 1;
    uint32_t exit_count;
    hx711_start(); //aqui cambiamos el estado del reloj a 0

    while(input_value_state == 1  && exit_count < LIMITWAITINGTIME){//esperamos a que la entrada cambie de high a low por el mismo
        exit_count++;
        input_value_state=read_input_state();
    }

    if(exit_count >= LIMITWAITINGTIME){
        *data_out = 0;
        exit_count=0;
        input_value_state = 1;
        hx711_deinit();
        return timeout_reached;
    }

    exit_count=0;
    input_value_state = 1;

    for(uint8_t i =0; i<24; i++){

        GPIO_Write(SCK_output_signal, 1); //encendemos el output
        data = (data<<1) | read_input_state(); //cuando este encendido hacemos es hacer pruebas de esto
        GPIO_Write(SCK_output_signal, 0);
    }

    for (uint8_t j=0; j<gane_choose; j++){
        GPIO_Write(SCK_output_signal, 1);
        Delay(2);
        Delay(2);
        Delay(2);
        GPIO_Write(SCK_output_signal, 0);
        Delay(2);
        Delay(2);
        Delay(2);
    }

    if(data ==0){
        *data_out = 0;
        hx711_deinit();
        return timeout_reached;
    }

   *data_out = data ^ 0x800000;
   //aqui checar si el valor es cero

   hx711_deinit();

   return successful_task;
}


general_codes_t Read_Data(uint32_t* data_ciclo){ //leemos la informacion cruda

    general_codes_t status=successful_task;
    uint32_t data_sum=0;
    uint32_t data=0;
    uint8_t cuenta =0;
    uint8_t count_fail =0;

    while(cuenta<40 && count_fail<10){ //tomara 40 lecturas validas

            status = hx711_read(&data);

            if(status != successful_task){
                count_fail++;
            }else{
                data_sum+=data;
                cuenta++;
            }

    }

    if(count_fail >= 10){
        data_sum=0;
        data = 0;
        cuenta = 0;
        count_fail=0;
        (*data_ciclo) = 0;
        return timeout_reached;
    }

    (*data_ciclo)=data_sum/cuenta;

    data_sum=0;
    data = 0;
    cuenta = 0;
    count_fail=0;

   return status;
}

//general_codes_t Get_Weight(int32_t* weight){//aqui esta la ecuacion sacada de los datos del gyn
//    general_codes_t status;
//    float real_weight =0;
//    uint32_t ADC=0;
//    status = Read_Data(&ADC);
//
//    if(successful_task != status || ADC==0){
//        *weight = 4242;
//        return status;
//    }
//    real_weight =(0.468888f *ADC)-(0.468888f*adc_cero);//esto es habiendo colocado funcion de el cero fijo
//    //real_weight =(0.468888f*adc_peso)-398444.0f;//usando el cero fijo que se obtuvo en gym
//
//    (*weight) = ((int32_t)real_weight)/10;
//
//    return status;
//}

void download_parameters_cali(uint32_t* p0,uint32_t* p1, uint32_t *p2){

    adc_cero  = (*p0);
    adc_peso1 = (*p1);
    adc_peso2 = (*p2);
//    adc_peso3 = (*p3);

}

general_codes_t Get_Weight(int32_t* weight){
    general_codes_t status=successful_task;
    float real_weight =0;
    uint32_t ADC=0;
    status = Read_Data(&ADC);

    if(successful_task != status || ADC==0){
        *weight = 0;
        return status;
    }

    real_weight =(var_ref *ADC)-(var_ref*adc_cero);//esto es habiendo colocado funcion de el cero fijo

    (*weight) = ((int32_t)real_weight)/10;

    return status;
}


general_codes_t Calibracion_pesos(select_weight peso,uint32_t *var_peso){

    general_codes_t status;
    uint32_t data_ciclo =0;
    uint8_t good_read =0;
    uint8_t exit_cicle =0;

    do{
        status=Read_Data(&data_ciclo);
        if(data_ciclo>= (adc_cero + extra)){
            good_read++;
        }else{
            good_read=0;
            exit_cicle++;
        }

        if(successful_task != status){
            return status;
        }
        if(exit_cicle >= 5){ //antes 20
            *var_peso=0;
            return timeout_reached;
        }

    }while(peso != peso_0 && good_read<3);


    if(peso == peso_0){
        adc_cero = data_ciclo;
        *var_peso=data_ciclo;
        extra = adc_cero*0.005; //sacamos el 20% para tener margen de error
    }
    else if (peso == peso_1){///**************************************///peso1

        adc_peso1 =data_ciclo;
        *var_peso=data_ciclo;
        extra = adc_peso1*0.005;

    }
    else{//*****************************peso2
        adc_peso2=data_ciclo;
        *var_peso=data_ciclo;
        extra =0;
    }
//    else{
//        adc_peso3 = data_ciclo;
//        *var_peso=data_ciclo;
//    }
        data_ciclo=0;
        good_read =0;

    return status;
}


void calibration_factor(void){

    adc_peso3=((adc_peso2-adc_peso1)/(peso2-peso1))*(peso3-peso1)+adc_peso1;
    adc_peso4=((adc_peso3-adc_peso2)/(peso3-peso2))*(peso4-peso2)+adc_peso2;

//      adc_peso5=((adc_peso2-adc_peso1)/(peso2-peso1))*(peso5-peso1)+adc_peso1;
//      adc_peso6=((adc_peso3-adc_peso2)/(peso3-peso2))*(peso6-peso2)+adc_peso2;
//      adc_peso7=((adc_peso4-adc_peso3)/(peso4-peso3))*(peso7-peso3)+adc_peso3;
//      float hasta=0;


    var_ref = 0.0f;
    var_ref +=((peso1-peso_cero)/(adc_peso1-adc_cero));
    var_ref +=((peso2-peso1)/(adc_peso2-adc_peso1));
//    printf("%.5f\n",var_ref);
    var_ref +=((peso3-peso2)/(adc_peso3-adc_peso2));
    var_ref +=((peso4-peso3)/(adc_peso4-adc_peso3));

    var_ref = (var_ref/4)*10;//para dejarlo en formato 0.468 aprox

}




