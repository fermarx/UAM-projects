# Prog de prueba para Practica 2. Ej 2

.data 0
    num0: .word 1 # posic 0
    num1: .word 2 # posic 4
    num2: .word 4 # posic 8
    num3: .word 8 # posic 12
    num4: .word 16 # posic 16
    num5: .word 32 # posic 20
    num6: .word 271828 # posic 24
    num7: .word 314159 # posic 28
    num8: .word 0 # posic 32
    num9: .word 0 # posic 36
    num10: .word 0 # posic 40
    num11: .word 0 # posic 44
.text 0
main:
    lw $t0, 20($zero) # Guarda un 32 en 8
    lw $t1, 20($zero) # Guarda un 32 en 9
    beq $t0, $t1, salto_e_l # Compara 8 con 9 (Salto efectivo)
    lw $t3, 24($zero) # Guarda un 271828 en 11 (No debería ejecutarse)
    lw $t4, 24($zero) # Guarda un 271828 en 11 (No debería ejecutarse)
    lw $t5, 24($zero) # Guarda un 271828 en 11 (No debería ejecutarse)

    salto_e_l:
        lw $t1, 16($zero) # Guarda un 16 en 9
        beq $t1, $t0, salto_n_l # Compara 8 con 9 (Salto no efectivo)
        lw $t3, 0($zero) # Guarda un 1 en 11

    salto_n_l:
        add $t1, $t1, $t1 # Guarda un 32 en 9
        beq $t1, $t0, salto_e_r # Compara 8 con 9 (Salto efectivo)
        lw $t3, 28($zero) # Guarda un 314159 en 12 (No debería ejecutarse)
        lw $t4, 28($zero) # Guarda un 314159 en 12 (No debería ejecutarse)
        lw $t5, 28($zero) # Guarda un 314159 en 12 (No debería ejecutarse)

    salto_e_r:
        add $t1, $t1, $t1 # Guarda un 64 en 9
        beq $t1, $t0, salto_n_r # Compara 8 con 9 (Salto no efectivo)
        lw $t4, 0($zero) # Guarda un 1 en 12

    salto_n_r:
        beq $zero, $zero, salto_n_r # Fin de programa
