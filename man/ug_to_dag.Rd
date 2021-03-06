% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/utils.R
\name{ug_to_dag}
\alias{ug_to_dag}
\title{Moral DAG from non chordal UG}
\usage{
ug_to_dag(ug)
}
\arguments{
\item{ug}{An igraph undirected graph.}
}
\value{
An igraph acyclic directed graph orientation.
}
\description{
Find the DAG with no v-structures whose skeleton is a
triangulation of a given undirected graph.
}
