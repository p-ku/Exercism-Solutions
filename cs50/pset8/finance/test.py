import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

db = SQL("sqlite:///finance.db")
funds_available = db.execute("SELECT cash FROM users WHERE id = :uid",uid=session["user_id"])
print(type(lookup("AAPL")))