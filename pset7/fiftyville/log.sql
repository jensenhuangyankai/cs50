-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street"; 
--10:15am
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;
--161|Ruth|2021|7|28|Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
--162|Eugene|2021|7|28|I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
--163|Raymond|2021|7|28|As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 30;

SELECT * FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC;

SELECT * FROM airports WHERE id = 4;

SELECT * FROM atm_transactions WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 and transaction_type = "withdraw";

SELECT * FROM phone_calls WHERE  year = 2021 AND month = 7 AND day = 28 AND duration < 60;

SELECT * FROM passengers WHERE flight_id = 36;

SELECT * FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 and transaction_type = "withdraw");
--thief finding
SELECT * FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE  year = 2021 AND month = 7 AND day = 28 AND duration < 60) 
    AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 30)
    AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36)
    AND id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 and transaction_type = "withdraw"));

SELECT receiver FROM phone_calls WHERE caller = "(367) 555-5533" AND year = 2021 AND month = 7 AND day = 28 AND duration < 60;

--accomplice finding
SELECT * FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE caller = "(367) 555-5533" AND year = 2021 AND month = 7 AND day = 28 AND duration < 60);