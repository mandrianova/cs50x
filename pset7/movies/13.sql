SELECT name from people
WHERE id in
      (SELECT person_id FROM stars WHERE movie_id in
          (SELECT id FROM movies WHERE id in
              (SELECT movie_id FROM stars WHERE person_id in
                  (SELECT id FROM people WHERE name = "Kevin Bacon" and birth = 1958))))
AND name != "Kevin Bacon"
GROUP BY id;