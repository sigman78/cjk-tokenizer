package Lingua::CJK::Tokenizer;

use strict;
use XSLoader;

XSLoader::load 'Lingua::CJK::Tokenizer';

1;
__END__

=pod

=head1 NAME

Locale::Memories - L10N Message Retrieval

=head1 SYNOPSIS

    my $tknzr = Lingua::CJK::Tokenizer->new();
    $tknzr->ngram_size(5);
    $tknzr->max_token_count(100);
    $tokens_ref = $tknzr->tokenize("CJK Text");

=head1 COPYRIGHT

Copyright (c) 2007 Yung-chung Lin. 

This program is free software; you can redistribute it and/or modify
it under the MIT License.

=cut
