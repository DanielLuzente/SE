# Projeto Software Embarcado

## Descrição

Desenvolver um sistema de controle automatizado de iluminação e refrigeração
de um Datacenter utilizando arduínos, sensores e atuadores. Objetivo principal
do projeto é fazer dois sistemas distintos, onde um coleta e transmite as informações dos 
sensores para outro sistema que ficará responsável por receber as informações e tomar as 
decisões de atuação caso as condições pré-definidas sejam satisfeitas.

## Componentes do sistema

**Sensores** - Botões, AM2302 (Temperatura e umidade), Flex Sensor (Flexão e Deflexão) e PIR Sensor (Movimento)

**Atuadores** - Leds, relé e Speaker

## Comunicação via Rádio Frequência

Para prover a comunicação entre os sistemas serão utilizados dois módulos transceivers NRF24L01,
que terão funções de transmissão dos dados coletados dos sensores para o outro sistema que terá
o segundo módulo trabalhando como receptor.
