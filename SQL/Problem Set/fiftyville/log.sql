-- Keep a log of any SQL queries you execute as you solve the mystery.

-- At first I wanted to find the Crime description to see what im working with. Found out (10:15am / bakery / 3 witnesses).
SELECT description
FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- Wanted to see what activity means. Found out (entrance / exit).
SELECT activity
FROM bakery_security_logs;

-- Wanted to find out if someone left at 10:15am. Found out (exit).
SELECT activity
FROM bakery_security_logs
WHERE hour = 10 AND minute = 15;

-- Wanted to search for relevant interviews at that day. Found out (Within 10 minutes drove away / Thief was at ATM Leggett Street / Took earliest flight next day. accomplice bought ticket / called for less then a minute).
SELECT transcript
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';

-- Wanted to count how many licence plates were recorded from 10:15 - 10:25am.
SELECT license_plate
FROM bakery_security_logs
WHERE hour = 10 AND minute >= 15 AND minute <= 25;

-- Wanted to see the names of all possible suspects. Found out suspects are (Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana, Kelsey, Noah, Kathleen, Alice, Christine, Karen, Alexander).
SELECT name
FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit';

-- Wanted to see what transaction_type is. Found out (deposit / withdraw)
SELECT transaction_type
FROM atm_transactions;

-- Identify suspects based on ATM withdrawals since I knew ATM location. Found out suspects are (Bruce, Diana, Iman, Luca) because (Brooke, Kenny, Taylor, Benista) were not parked near bakery.
SELECT name
FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28
  AND atm_transactions.atm_location = 'Leggett Street' AND atm_transactions.transaction_type = 'withdraw';

-- Wanted to find out passport_numbers of suspects. Found out (Iman: 7049073643 / Luca: 8496433585 / Diana: 3592750733 / Bruce: 5773159633).
SELECT name, passport_number
FROM people
WHERE name IN ('Luca', 'Bruce', 'Iman', 'Diana');

-- See all aiports in Fiftyville. Found out (Fiftyville Regional Airport).
SELECT full_name
FROM airports
WHERE city = 'Fiftyville';

-- Get abbreviation of airport. Found out (CSF).
SELECT abbreviation
FROM airports
WHERE full_name = 'Fiftyville Regional Airport';

-- Get id of CSF airport. Found out (id: 8).
SELECT id
FROM airports
WHERE abbreviation = 'CSF';

-- Get names of people that were on a the given flight. Found out suspects are (Bruce, Luca) Because (Doris, Sofia, Edward, Kelsey, Taylor, Kenny) were not in our prior list.
SELECT name
FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = (
    SELECT id
    FROM flights
    WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8
    ORDER BY hour, minute ASC
    LIMIT 1
);

-- Get name of the thief by looking at all people who called. Found out thief is (Bruce) because (Sofia, Kelsey, Kathryn, Taylor, Diana, Carina, Kenny, Benista) were not in the prior suspect list.
SELECT name
FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND duration <= 60;

-- Find out Where the thief escaped to. Found out (New York City).
SELECT city
FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8
ORDER BY hour, minute ASC
LIMIT 1;

-- Find out number of Bruce. Found out ((367) 555-5533).
SELECT phone_number FROM people WHERE name = 'Bruce';

-- Find out the name of the accomplice. Found out (Robin).
SELECT name
FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE year = 2021 AND month = 7 AND day = 28 AND phone_calls.duration <= 60
  AND phone_calls.caller = '(367) 555-5533';