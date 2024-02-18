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
            if !firstq.contains("∈") {
                temp.remove("∈");
                break;
            }
        }
        result = result.union(&temp).cloned().collect();
    }
    result
}

fn follow(grammar: &Grammar, symbol: &String) -> HashSet<String> {
    let mut result = HashSet::new();
    if symbol == &grammar.start {
        result.insert("$".to_string());
    }
    for (lhs, rules) in &grammar.rules {
        for rule in rules {
            for (i, q) in rule.iter().enumerate() {
                if q == symbol {
                    let mut j = i;
                    let mut temp = HashSet::from(["∈".to_string()]);
                    while temp.contains("∈") {
                        temp.remove("∈");
                        result = result.union(&temp).cloned().collect();
                        j += 1;
                        if j == rule.len() {
                            if lhs != symbol {
                                result = result.union(&follow(grammar, lhs)).cloned().collect();
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
    let filename = env::args().nth(1).unwrap_or("grammar.txt".to_string());
    for line in std::fs::read_to_string(filename)
        .expect("No such file")
        .lines()
    {
        if grammar.start.is_empty() {
            grammar.start = line.split_whitespace().next().unwrap().to_string();
        }
        let lhs = line.split_whitespace().next().unwrap();
        if !lhs.starts_with('<') || !lhs.ends_with('>') {
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
                "===>" => continue,
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
            if line.starts_with('<') && word.ends_with('>') {
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
    // dbg!(&grammar);
    for nt in &grammar.non_terminals {
        println!("{}: {:?}", nt, first(&grammar, nt));
    }
    println!();
    for nt in &grammar.non_terminals {
        println!("{}: {:?}", nt, follow(&grammar, nt));
    }
}
