import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
import math

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")
db.execute("CREATE TABLE IF NOT EXISTS stocks (id INTEGER NOT NULL,symbol VARCHAR(255) NOT NULL, quantity INTEGER NOT NULL, price FLOAT,time_bought timestamp ,FOREIGN KEY (id) REFERENCES users(id))")
db.execute("CREATE TABLE IF NOT EXISTS history (id INTEGER NOT NULL, time timestamp ,symbol VARCHAR(255) NOT NULL, action VARCHAR(255) NOT NULL, quantity INTEGER NOT NULL, price FLOAT, total_price FLOAT, FOREIGN KEY (id) REFERENCES users(id))")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    id = session["user_id"]
    items = db.execute("SELECT * FROM stocks WHERE id = ?",id)
    for i in items:
        i.pop("id")
    print(items)
    money = float(db.execute("SELECT cash FROM users WHERE id =?",id)[0]["cash"])
    return render_template("index.html",items=items, money=money)

symbol = ""
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    id = session["user_id"]
    money = float(db.execute("SELECT cash FROM users WHERE id =?",id)[0]["cash"])
    if request.method == "GET":
        return render_template("buy.html", money = money)

    global symbol
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quantity = request.form.get("shares")
        if symbol == "":
            return apology("Symbol cannot be empty!")
        answer = lookup(symbol)
        if answer == None:
            return apology("Recheck the validity of your symbol!")
        if symbol:
            symbol = request.form.get("symbol").upper()
            price = lookup(symbol)["price"]
            #return render_template("buy.html",money=money, price=price, symbol=symbol, count=math.floor(money/price))
        else:
            return apology("Symbol empty!")



        price = float(lookup(symbol)["price"])
        
        print()
        if quantity.isdigit() == False:
            return apology("quantity is not integer!")
        quantity = int(quantity)
        print(price*quantity)
        if (price * quantity < money):
            money = money - price*quantity
            db.execute("UPDATE users SET cash=? WHERE id =?",money,id)
            time_bought = datetime.now()
            time_bought = time_bought.strftime("%m/%d/%Y, %H:%M:%S")
            db.execute("INSERT INTO history VALUES(?,?,?,?,?,?,?)",id,time_bought,symbol,"BUY",quantity,price,price*quantity)

            if db.execute("SELECT id from stocks WHERE symbol=?",symbol) != []: #stock exists
                existing_record = db.execute("SELECT * FROM stocks WHERE symbol=? AND id = ?",symbol,id)
                print(existing_record)
                existing_amount = int(existing_record[0]["quantity"])
                existing_price = float(existing_record[0]["price"])
                new_price = (price*quantity + existing_amount*existing_price)/(existing_amount+quantity)
                db.execute("UPDATE stocks SET quantity = ?, time_bought = ? ,price=? WHERE id = ? AND symbol = ?",quantity+existing_amount,time_bought,price,id,symbol)
            else:
                db.execute("INSERT INTO stocks VALUES (?,?,?,?,?)",id,symbol,quantity,price,time_bought)
            return render_template("buy.html",money=money, price=price, symbol=symbol, count=math.floor(money/price), total_cost = quantity*price)

        


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    items = db.execute("SELECT * FROM history WHERE id = ?",id)
    for i in items:
        i.pop("id")
    #print(items)
    return render_template("history.html",items=items)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        print("???")
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if symbol == "":
            return apology("Symbol cannot be empty!")
        answer = lookup(symbol)
        if answer == None:
            return apology("Recheck the validity of your symbol!")
        return render_template("quoted.html", price=answer["price"], symbol=answer["symbol"]) #include more parameters from lookup here.


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        #print(password,confirm_password)
        if (db.execute("SELECT username FROM users where username = ?", username)):
            return apology("username already taken!")

        if password != confirmation:
            return apology("check your confirmation")
        if username == "" or password == "" or confirmation == "":
            return apology("username/password is empty!")
        hashed = generate_password_hash(password)
        #print(username,hashed)
        db.execute("INSERT INTO users (username,hash) VALUES (?,?)",username,hashed)

        
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    id = session["user_id"]
    items = db.execute("SELECT * FROM stocks WHERE id = ?",id)
    for i in items:
        i.pop("time_bought")
        i.pop("id")

    result = db.execute("SELECT symbol FROM stocks WHERE id = ?", id)
    print(result)
    all_symbols = []
    for thing in result:
        all_symbols.append(thing['symbol'])

    """Show portfolio of stocks"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        print(symbol)
        sell_quantity = request.form.get("shares")
        if sell_quantity.isdigit() == False:
            return apology("quantity is not integer!")
        else:
            sell_quantity=int(sell_quantity)
        if sell_quantity > 0:
            sell_price= float(lookup(symbol)["price"])
            if sell_price != "":
                sell_price = float(sell_price)
            else:
                return render_template("sell.html",items=items)
            
            print(symbol)
            entry = db.execute("SELECT * FROM stocks WHERE symbol = ?",symbol)
            if entry != []:
                entry = entry[0]
            else:
                return render_template("sell.html",items=items)
            initial_quantity = int(entry["quantity"])
            if sell_quantity <= initial_quantity:
                final_quantity = initial_quantity - sell_quantity
                if final_quantity > 0:
                    db.execute("UPDATE stocks SET quantity = ? WHERE id =? and symbol = ?",final_quantity , id, symbol)
                else:
                    print("stonk quantity is zero so deleting")
                    db.execute("DELETE FROM stocks WHERE id = ? and symbol = ?", id,symbol)
                time_sold = datetime.now()
                time_sold = time_sold.strftime("%m/%d/%Y, %H:%M:%S")
                initial_money = db.execute("SELECT cash FROM users WHERE id = ?",id)[0]['cash']
                print(initial_money)
                final_money = initial_money + sell_price*sell_quantity
                db.execute("UPDATE users SET cash=? WHERE id =?",final_money,id)
                db.execute("INSERT INTO history VALUES(?,?,?,?,?,?,?)",id,time_sold,symbol,"SELL",sell_quantity,sell_price,sell_price*sell_quantity)
                return redirect(url_for('index'))
                return render_template("sell.html",items=items)
            else:
                return apology("selling too many!")
        
    if request.method == "GET":
        
        return render_template("sell.html",items=items,symbols=all_symbols)
