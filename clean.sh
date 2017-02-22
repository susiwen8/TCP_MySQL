#!/bin/bash

cmd="cd server/"
$cmd
cmd="make clean"
$cmd
cmd="cd ../client/"
$cmd
cmd="make clean"
$cmd
