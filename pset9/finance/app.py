import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
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
db.execute("CREATE TABLE IF NOT EXISTS history (id INTEGER NOT NULL, time_bought timestamp ,symbol VARCHAR(255) NOT NULL, action VARCHAR(255) NOT NULL, quantity INTEGER NOT NULL, price FLOAT, total_price FLOAT, FOREIGN KEY (id) REFERENCES users(id))")

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
    return render_template("index.html",items=items)

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
        if request.form.get("quantity"):
            price = float(lookup(symbol)["price"])
            quantity = int(request.form.get("quantity"))
            
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
                return render_template("buy.html",money=money, price=price, symbol=symbol, count=math.floor(money/price))

        if request.form.get("symbol"):
            symbol = request.form.get("symbol").upper()
            price = lookup(symbol)["price"]
            return render_template("buy.html",money=money, price=price, symbol=symbol, count=math.floor(money/price))


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
        answer = lookup(symbol)
        return render_template("quoted.html", price=answer["price"], symbol=answer["symbol"]) #include more parameters from lookup here.


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirm-password")
        #print(password,confirm_password)
        if (db.execute("SELECT username FROM users where username = ?", username)):
            return apology("username already taken!")

        if password == confirm_password and username != "":
            hashed = generate_password_hash(password)
            #print(username,hashed)
            db.execute("INSERT INTO users (username,hash) VALUES (?,?)",username,hashed)

        
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    id = session["id"]
    names = db.execute("SELECT symbol FROM stocks WHERE id = ?",id)

    return render_template("sell.html",names=names)
