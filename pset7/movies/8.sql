SELECT name FROM people
JOIN stars s on people.id = s.person_id
JOIN movies m on s.movie_id = m.id
WHERE title = "Toy Story";