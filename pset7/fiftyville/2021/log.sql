-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT * FROM crime_scene_reports; //mentions courthouse

SELECT license_plate FROM courthouse_security_logs WHERE activity = exit AND year = 2020 AND month = 7 AND day = 28;

SELECT name,transcript FROM interviews WHERE year = 2020 AND month = 7 AND day = 28;
SELECT description FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28;

//find destination flight
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1);

//find flight number
SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1

//find info about call?
SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60;

//thief was at Fifer Street withdrawing money in morning
SELECT account_number,transaction_type,atm_location,amount FROM atm_transactions WHERE transaction_type = "withdraw" AND atm_location = "Fifer Street" AND year = 2020 AND month = 7 AND day = 28;

//link phone number to person to flight passport_number
SELECT id FROM people WHERE passport_number IN
    (
        SELECT passport_number FROM passengers WHERE flight_id = 
            (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1)
    INTERSECT
        SELECT passport_number FROM people WHERE phone_number IN  
            (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
    )
;
//5 suspects

//narrowed down to 2 suspects. dang
SELECT id FROM people WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs WHERE activity = "exit" AND year = 2020 AND month = 7 AND day = 28)
INTERSECT
    SELECT id FROM people WHERE passport_number IN
    (
        SELECT passport_number FROM passengers WHERE flight_id = 
            (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1)
    INTERSECT
        SELECT passport_number FROM people WHERE phone_number IN  
            (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
    )
INTERSECT
--this following line finds the people that withdrew from the atm on Fifer St
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND atm_location = "Fifer Street" AND year = 2020 AND month = 7 AND day = 28)
;

--FOUND THE NAME OF THE GUY!!! time to find his friend's number.
SELECT name FROM people WHERE id =
(SELECT id FROM people WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs WHERE activity = "exit" AND year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute < 30)
INTERSECT
    SELECT id FROM people WHERE passport_number IN
    (
        SELECT passport_number FROM passengers WHERE flight_id = 
            (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1)
    INTERSECT
        SELECT passport_number FROM people WHERE phone_number IN  
            (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
    )
INTERSECT
--this following line finds the people that withdrew from the atm on Fifer St
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND atm_location = "Fifer Street" AND year = 2020 AND month = 7 AND day = 28)
)
;

SELECT name FROM people WHERE phone_number =
(SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND caller =
(SELECT phone_number FROM people WHERE id =
(SELECT id FROM people WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs WHERE activity = "exit" AND year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute < 30)
INTERSECT
    SELECT id FROM people WHERE passport_number IN
    (
        SELECT passport_number FROM passengers WHERE flight_id = 
            (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC LIMIT 1)
    INTERSECT
        SELECT passport_number FROM people WHERE phone_number IN  
            (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
    )
INTERSECT
--this following line finds the people that withdrew from the atm on Fifer St
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND atm_location = "Fifer Street" AND year = 2020 AND month = 7 AND day = 28)
)))
;