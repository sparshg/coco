// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)

use core::panic;
use std::{
    collections::{HashMap, HashSet},
    env,
};
#[derive(Debug)]

struct Grammar {
    start: String,
    terminals: HashSet<String>,
    non_terminals: HashSet<String>,
    rules: HashMap<String, Vec<Vec<String>>>,
}

fn first(grammar: &Grammar, symbol: &String) -> HashSet<String> {
    if grammar.terminals.contains(symbol) {
        return [symbol.clone()].iter().cloned().collect();
    }
    let mut result = HashSet::new();
    for rule in grammar
        .rules
        .get(symbol)
        .expect(&format!("{} not found", symbol))
    {
        let mut temp = HashSet::new();
        for q in rule {
            let firstq = first(grammar, q);
            temp = temp.union(&firstq).cloned().collect();
            if !firstq.contains("#") {
                temp.remove("#");
                break;
            }
        }
        result = result.union(&temp).cloned().collect();
    }
    result
}

fn follow(grammar: &Grammar, symbol: &String, track: &mut HashSet<String>) -> HashSet<String> {
    let mut result = HashSet::new();
    if track.contains(symbol) {
        return result;
    }
    if symbol == &grammar.start {
        result.insert("$".to_string());
    }
    for (lhs, rules) in &grammar.rules {
        for rule in rules {
            for (i, q) in rule.iter().enumerate() {
                if q == symbol {
                    let mut j = i;
                    let mut temp = HashSet::from(["#".to_string()]);
                    while temp.contains("#") {
                        temp.remove("#");
                        result = result.union(&temp).cloned().collect();
                        j += 1;
                        if j == rule.len() {
                            if lhs != symbol {
                                track.insert(symbol.clone());
                                result = result
                                    .union(&follow(grammar, lhs, track))
                                    .cloned()
                                    .collect();
                                track.remove(symbol);
                            }
                            break;
                        }
                        temp = first(grammar, &rule[j]);
                    }
                    result = result.union(&temp).cloned().collect();
                }
            }
        }
    }
    result
}

fn main() {
    let mut grammar = Grammar {
        start: String::new(),
        terminals: HashSet::new(),
        non_terminals: HashSet::new(),
        rules: HashMap::new(),
    };
    let file = std::fs::read_to_string(env::args().nth(1).unwrap_or("grammar.txt".to_string()))
        .expect("No such file");
    for line in file.lines() {
        if grammar.start.is_empty() {
            grammar.start = line.split_whitespace().next().unwrap().to_string();
        }
        let lhs = line.split_whitespace().next().unwrap();
        if lhs.chars().next().unwrap().is_uppercase() {
            panic!("Invalid non-terminal: {} on LHS", lhs);
        }
        grammar.non_terminals.insert(lhs.to_string());
        grammar
            .rules
            .entry(lhs.to_string())
            .or_insert_with(Vec::new)
            .push(Vec::new());
        for word in line.split_whitespace().skip(1) {
            match word {
                "=" => continue,
                "|" => {
                    grammar
                        .rules
                        .entry(lhs.to_string())
                        .or_insert_with(Vec::new)
                        .push(Vec::new());
                    continue;
                }
                _ => (),
            }
            if word.chars().next().unwrap().is_lowercase() {
                grammar.non_terminals.insert(word.to_string());
            } else {
                grammar.terminals.insert(word.to_string());
            }
            grammar
                .rules
                .entry(lhs.to_string())
                .and_modify(|v| v.last_mut().unwrap().push(word.to_string()));
        }
    }
    dbg!(&grammar);
    let mut track = HashSet::new();
    for line in file.lines() {
        println!(
            "{}",
            format!(
                "{:?} : {:?}",
                // line,
                {
                    let mut temp = HashSet::new();
                    for q in line.split_whitespace().skip(2) {
                        let firstq = first(&grammar, &q.to_string());
                        temp = temp.union(&firstq).cloned().collect();
                        if !firstq.contains("#") {
                            temp.remove("#");
                            break;
                        }
                    }
                    temp
                },
                follow(
                    &grammar,
                    &line.split_whitespace().next().unwrap().to_string(),
                    &mut track
                )
            )
            .replace(&['\"', ',', '{', '}'][..], "")
        );
    }
}