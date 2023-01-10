SELECT city.city_id,city.city
FROM   city,(SELECT address.address_id,address.city_id
        FROM   address,(SELECT store.store_id, store.address_id
                FROM   store, (SELECT inventory_id,    inventory.store_id
                        FROM   inventory,  (SELECT film.film_id
                                FROM   film, (SELECT film_id
                                        FROM   film_actor,  (SELECT actor_id, first_name
                                                FROM   actor
                                                WHERE  first_name = 'Bob'  AND last_name = 'Fawcett'
                                               ) AS actor_bob -- SELECTS THE NAME BOB AND LAST NAME FAWCETT 
                                        WHERE  film_actor.actor_id =   actor_bob.actor_id) AS film_of_bob --SELECTS ALL THE FILMS IN WICH HE APPEARS 
                                WHERE  film.film_id = film_of_bob.film_id) AS inventory_bob --SELECTS AL THE MOVIES OF THE INVENTORY HE APPEARS 
                        WHERE  inventory.film_id = inventory_bob.film_id) AS  store_bob
                WHERE  store_bob.store_id = store.store_id) AS store_id_bob --SELECTS ALL THE STORES THAT HAS THE MOVIES HE APPEARS 
        WHERE  store_id_bob.address_id = address.address_id) AS city_bob --SELECTS THE CITY ID IN WICH THE STORE THAT HAS HIS FILMS ARE 
WHERE  city_bob.city_id = city.city_id
GROUP  BY city.city_id ORDER BY city.city ASC -- SELECTS THE NAME OF THE CITIES IN ALPHABETICAL ORDER 