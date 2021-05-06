# App para IOT - Grupo 8
### Disciplina de Dispositivos Móveis - IMD0509 2020.2
Para a prova de reposição, o Grupo 8, formado por:
- Cleyton da Silva Vitor
- Gdiael Souto Barros
- Mariano José da Silva Filho

Tentamos implementar parte do que foi proposto no trabalho que segunda unidade da disciplina, que consistia em um orçamento para o desenvolvimento de um aplicativo para gerência de dispositivos inteligêntes, também conhecido com IOT (Internet das Coisas).
O aplicativo desenvolvido foi o mais simples possível, devivo ao pouco espaço de tempo disponível para a implementação do mesmo, ele consiste basicamente de três partes, um módulo de Login utilizando a plataforma FireBase da Google, um módudo de comunicação com um Broker Mqtt, ambos em Kotlin e um módulo externo, em C++ para comunicação com um arduino.
Foi feito um vídeo para o demonstração das capacidades de Login, usando a autenticação pelo Google e de comunição entre o app e dois dispositivos, um sensor de temperatura e uma luz led.

https://youtu.be/lFkzIwPl8hE

repositório do git hub, com o código fonte do app
https://github.com/gdiael/MqttIOT_Grupo8.git

Bíblioteca Paho utilizada para comunicação com o Broker
https://github.com/eclipse/paho.mqtt.android
