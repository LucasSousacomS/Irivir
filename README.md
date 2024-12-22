# Irivir

## Objetivo
Eu tenho uma gatinha que, no momento, se encontra com um peso um pouco elevado. Ela passa o dia inteiro deitada e só levanta pra comer. São muitos raros os momentos em que ela brinca, fazendo com que ela só cresça mais e mais para os lados. Seria possível comprar um laser pra brincar com ela? Com certeza. Porém, não seria tão emocionante quanto fazer um carrinho automático que foge dela e desvia de obstáculos

## Funcionamento (planejado)
Um carrinho que, ao detectar a presença de algo via sensor PIR, inicia uma rotina de movimento. Um sensor de distância laser conectado a um servo motor escaneia os arredores. Se detecta um obstáculo à 50 cm de distância em um lado, se movimenta para o outro (se encontra o obstáculo na direita, vira pra esquerda e vice versa). A cada tanto segundos, para de se movimentar. A quantidade de segundos é aleatória e ele pode tanto voltar a se mover sozinho aleatoriamente ou só quando detectar algo com o sensor PIR.